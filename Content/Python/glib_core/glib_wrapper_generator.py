import re

from CppHeaderParser import *


class GLibParserHelper:
    PREFIX_PATTERN = r"(^[A-Z](?=[A-Z].*$))*(.*)"

    @classmethod
    def get_name_without_prefix(cls, name):
        class_name_match = re.search(cls.PREFIX_PATTERN, name)
        return class_name_match.group(2) if class_name_match else name

    @classmethod
    def add_prefix(cls, name: str, settings: dict):
        if not settings or not settings.get("prefix_override"):
            return name

        name_match = re.search(cls.PREFIX_PATTERN, name)

        if name_match:
            prefix_letter = name_match.group(1)
            actual_class_name = name_match.group(2)

            new_name = prefix_letter + settings["prefix_override"] + actual_class_name
        else:
            new_name = settings["prefix_override"] + name

        return new_name

    @classmethod
    def get_valid_prefix_letter(cls, class_data: dict):
        prefix_letter = "F" if class_data.get("declaration_method") == "struct" else "U"
        return prefix_letter + class_data["name"][1:]


class GLibCppHeaderParser(CppHeader):
    def __init__(self, headerFileName, argType="file", encoding=None, delegates=None, settings: dict = None, **kwargs):
        self.delegates = delegates
        self.settings = settings
        self.name_overrides = dict()
        super().__init__(headerFileName, argType=argType, encoding=encoding, **kwargs)

    def remove_circular_refs(self, data, seen=None):
        if seen is None:
            seen = set()

        if id(data) in seen:
            # circular reference, remove it.
            return None

        seen.add(id(data))

        result = data

        if isinstance(data, dict):
            result = {self.remove_circular_refs(k, seen): self.remove_circular_refs(v, seen) for k, v in data.items()}
        elif isinstance(data, (list, tuple, set, frozenset)):
            result = type(data)(self.remove_circular_refs(v, seen) for v in data)

        # remove id again; only *nested* references count
        seen.remove(id(data))
        return result

    def toJSON(self, indent=4, separators=None):
        self.__dict__ = self.remove_circular_refs(self.__dict__)
        return super().toJSON(indent=indent, separators=separators)

    def finalize_vars(self):
        super(GLibCppHeaderParser, self).finalize_vars()

        for var in CppVariable.Vars:
            var["category"] = self.__get_category__(var.get("parent").get("name"))
            if var.get("typedef"):
                var["type"] = var["typedef"]

    def fixup_names(self, generated_data: str):
        for name, override in self.name_overrides.items():
            generated_data = generated_data.replace(name, override)

        return generated_data

    def _evaluate_class_stack(self):
        super(GLibCppHeaderParser, self)._evaluate_class_stack()
        if not self.settings:
            return

        current_class = self.classes.get(self.curClass)
        if not current_class:
            return

        current_class["name_override"] = GLibParserHelper.get_valid_prefix_letter(current_class)
        current_class["name_override"] = GLibParserHelper.add_prefix(current_class["name_override"], self.settings)
        self.name_overrides[current_class["name"]] = current_class["name_override"]

    def _evaluate_property_stack(self, clearStack=True, addToVar=None):
        super()._evaluate_property_stack(clearStack=clearStack, addToVar=addToVar)

        new_property = parseHistory[-1]["item"]
        if new_property.get("type") in self.delegates:
            new_property["delegate"] = True

            if self.settings and self.settings.get("prefix_override"):
                new_property["type"] = self.delegates[new_property["type"]]["name_override"]

    def _evaluate_method_stack(self):
        super(GLibCppHeaderParser, self)._evaluate_method_stack()

        last_method = parseHistory[-1]["item"]
        current_class = self.classes.get(self.curClass)
        if current_class:
            last_method["category"] = self.__get_category__(current_class.get("name"))

    def _install_enum(self, new_enum: CppEnum, instance_data):
        super(GLibCppHeaderParser, self)._install_enum(new_enum, instance_data)

        new_enum["name_override"] = GLibParserHelper.add_prefix(new_enum["name"], self.settings)
        self.name_overrides[new_enum["name"]] = new_enum["name_override"]

    def __get_category__(self, owner):
        parent_category = self.settings.get("parent_category")

        if owner:
            category_prefix = (parent_category + "|") if parent_category else ""
            return category_prefix + GLibParserHelper.get_name_without_prefix(owner)
        elif parent_category:
            return parent_category

        return None


class GLibBaseParser:
    @classmethod
    def __get_name__(cls, data):
        return data["name_override"] if data.get("name_override") else data["name"]


class GLibContainerParser(GLibBaseParser):
    @classmethod
    def parse(cls, data):
        result = cls.parse_properties(data)
        result += cls.parse_methods(data)

        result += "};\n"

        return result

    @classmethod
    def parse_properties(cls, data):
        result = ""
        for access_modifier, properties in data["properties"].items():
            if len(properties) == 0:
                continue

            result += access_modifier + ":\n"
            for class_property in properties:
                parsed_property = GLibPropertyParser.parse(class_property)

                if not parsed_property:
                    continue

                result += parsed_property + "\n"

        return result

    @classmethod
    def parse_methods(cls, data):
        result = ""
        number_methods = dict()
        for access_modifier, methods in data["methods"].items():
            if len(methods) == 0:
                continue

            result += access_modifier + ":\n"
            for class_method in methods:
                method_name = class_method["name"]
                if method_name in number_methods:
                    number_methods[method_name] += 1
                    parsed_function = GLibFunctionParser.parse(class_method, number_methods[method_name])
                else:
                    parsed_function = GLibFunctionParser.parse(class_method)

                if not parsed_function:
                    continue

                result += parsed_function + "\n"
                number_methods[method_name] = 1

        return result


class GLibClassParser(GLibContainerParser):
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "UCLASS(BlueprintType, Blueprintable)\n"
        result += "class " + cls.__get_name__(data) + " : public UObject\n"
        result += "{\n"
        result += "\tGENERATED_BODY()\n"
        result += "\n"

        result += super().parse(data)

        return result


class GLibStructParser(GLibContainerParser):
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "USTRUCT(BlueprintType, Blueprintable)\n"
        result += "struct " + cls.__get_name__(data) + "\n"
        result += "{\n"
        result += "\tGENERATED_BODY()\n"
        result += "\n"

        result += super().parse(data)

        return result


class GLibEnumParser(GLibBaseParser):
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "UENUM(BlueprintType)\n"
        result += "enum " + ("class " if data["isclass"] else "") + cls.__get_name__(data) + " : uint8" + "\n"
        result += "{\n"

        result += GLibEnumValueParser.parse_values(data)

        result += "};\n"
        return result


class GLibMemberParser(GLibBaseParser):
    @classmethod
    def get_doxygen(cls, data):
        return "\t" + data["doxygen"].replace("\n", "\n\t") + "\n" if data.get("doxygen") else ""


class GLibPropertyParser(GLibMemberParser):
    @classmethod
    def parse(cls, data):
        if data.get("mutable") or data.get("static"):
            return None

        result = cls.get_doxygen(data)

        if data.get("delegate"):
            result += "\tUPROPERTY(BlueprintAssignable, Category = \"{category}\")" + "\n"
        else:
            result += "\t" + "UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = \"{category}\")" + "\n"

        category = data["category"] if data.get("category") else "GLib"
            
        result = result.format_map({"category": category})
        variable_type = re.sub(r"static|enum|class|struct|const|\s", "", data["type"])
        static = "static " if data.get("static") else ""
        result += "\t" + static + variable_type + " " + cls.__get_name__(data) + ";\n"

        return result


class GLibFunctionParser(GLibMemberParser):
    DECLARATION_EXTRA_SPACES = r"(?:(?<=\()\s+)|(?:\s+((?=&|;|,|\)|\())+)|(?<=\d)\s*(?=\.)|(?<=\.)\s*(?=f)"

    @classmethod
    def parse(cls, data, index: int = None):
        if data.get("template"):
            return None

        result = cls.get_doxygen(data)

        if not data["constructor"]:
            result += "\tUFUNCTION({callable_type}, Category = \"{category}\")\n".format_map({
                "callable_type": "BlueprintPure" if data["rtnType"] != "void" and data["const"] else "BlueprintCallable",
                "category": data["category"] if data.get("category") else "GLib"
            })

        declaration = data["debug"]
        declaration = re.search(r".*\(.*\)(?:(?:\s*{.*})*)", declaration).group(0)
        declaration = re.sub(cls.DECLARATION_EXTRA_SPACES, "", declaration)

        if index and not data["constructor"]:
            parenthesis_index = declaration.find("(")
            declaration = declaration[:parenthesis_index] + str(index) + declaration[parenthesis_index:]

        result += "\t" + declaration + ";\n"

        return result


class GLibEnumValueParser(GLibMemberParser):
    @classmethod
    def parse(cls, data):
        result = cls.get_doxygen(data)
        result += "\t" + cls.__get_name__(data) + " = " + str(data["value"])

        return result

    @classmethod
    def parse_values(cls, data):
        result = ""
        for index, value in enumerate(data["values"]):
            result += GLibEnumValueParser.parse(value)

            if not data.get("isclass"):
                result += " UMETA(DisplayName = \"" + cls.__get_name__(data) + "\")"

            result += ",\n" if index < len(data["values"]) - 1 else "\n"

        return result


class GLibDelegateParser(GLibBaseParser):
    VOID_DELEGATE = r"DECLARE.*DELEGATE(?!.*RetVal).*"
    SPARSE_DELEGATE = r"DECLARE.*SPARSE.*DELEGATE.*"
    SPARSE_DELEGATE_PARAMS = r"(?:(?<=\().*?,.*?,.*?,)(.*(?=\)))"
    DELEGATE_NAME = r"(?<=\()\w*"
    DELEGATE_PARAMS = r"(?<=,).*(?=\))"
    DELEGATE_PARAMS_NUMBER = r"(?<=DELEGATE_).*?(?=Param[s]*\()"

    PARAMS_NUMBER_CONVERSION_DICT = dict({"one": 1, "two": 2, "three": 3, "four": 4, "five": 5, "six": 6, "seven": 7, "eight": 8, "nine": 9})


    @classmethod
    def parse(cls, raw_data, settings: dict = None):
        delegates = re.findall(cls.VOID_DELEGATE, raw_data)

        parsed_delegates = cls.parse_delegates(delegates, settings)
        parsed_delegates = cls.parse_params(parsed_delegates)

        return parsed_delegates

    @classmethod
    def parse_delegates(cls, delegates, settings: dict = None):
        parsed_delegates = dict()
        for delegate in delegates:
            delegate_name = re.search(cls.DELEGATE_NAME, delegate.replace(" ", "")).group(0)
            parsed_delegates[delegate_name] = dict()
            parsed_delegates[delegate_name]["name"] = delegate_name
            parsed_delegates[delegate_name]["name_override"] = GLibParserHelper.add_prefix(delegate_name, settings)
            parsed_delegates[delegate_name]["delegate"] = delegate
            parsed_delegates[delegate_name]["sparse"] = re.search(cls.SPARSE_DELEGATE, delegate) is not None
        return parsed_delegates

    @classmethod
    def parse_params(cls, parsed_delegates):
        for delegate_name, data in parsed_delegates.items():
            if data["sparse"]:
                delegate_params_match = re.search(cls.SPARSE_DELEGATE_PARAMS, data["delegate"])
                params = delegate_params_match.group(1) if delegate_params_match else None
            else:
                delegate_params_match = re.search(cls.DELEGATE_PARAMS, data["delegate"])
                params = delegate_params_match.group(0) if delegate_params_match else None

            if not params:
                data["params_number"] = 0
                return

            params_number_word = re.search(cls.DELEGATE_PARAMS_NUMBER, data["delegate"]).group(0)
            data["params_number_word"] = params_number_word

            params_number = cls.PARAMS_NUMBER_CONVERSION_DICT.get(params_number_word.lower())
            data["params_number"] = params_number

            params_list = params.split(",")

            if len(params_list) != params_number * 2:
                params = ""
                for index, param in enumerate(params_list):
                    params += param + ", Param" + str(index + 1)
                    if len(params_list) - 1 != index:
                        params += ", "

            data["params"] = params.strip()

        return parsed_delegates

    @classmethod
    def crete_multicast_delegates(cls, parsed_delegates, settings: dict = None):
        result = ""

        if settings and settings.get("sort_delegates"):
            delegates = dict()
            for delegate_name, data in parsed_delegates.items():
                new_delegate = cls.__create_multicast_delegate__(data)

                params_number = str(data["params_number"])
                if not delegates.get(params_number):
                    delegates[params_number] = list()

                delegates[params_number].append(new_delegate)

            for delegates_id, delegates in sorted(delegates.items()):
                for delegate in delegates:
                    result += delegate
        else:
            for delegate_name, data in parsed_delegates.items():
                new_delegate = cls.__create_multicast_delegate__(data)
                result += new_delegate

        return result

    @classmethod
    def __create_multicast_delegate__(cls, data: dict):
        new_delegate = "DECLARE_DYNAMIC_MULTICAST_DELEGATE"

        params = data.get("params")

        if params:
            params_number = re.search(cls.DELEGATE_PARAMS_NUMBER, data["delegate"]).group(0)
            params_count = params.split(",")
            new_delegate += f"_{params_number}" + ("Param" if len(params_count) == 2 else "Params")

        new_delegate += f"({cls.__get_name__(data)}" + (", " + params if params else "") + ");\n"
        return new_delegate


class GLibWrapperGenerator:
    @classmethod
    def parse(cls, path: str, settings: dict = None):
        if not os.path.exists(path):
            raise Exception("Path does not exist")

        with open(path, "r") as file:
            data = file.read()

        parsed_delegates = GLibDelegateParser.parse(data, settings)

        data = re.sub(r"\n*\s*(?:UCLASS|UENUM|UFUNCTION|UPROPERTY|USTRUCT|GENERATED.*BODY|UE_DEPRECATED|UMETA)\(.*\)", "", data)
        data = re.sub(r"(?:PRAGMA_ENABLE_DEPRECATION_WARNINGS|PRAGMA_DISABLE_DEPRECATION_WARNINGS).*", "", data)
        data = re.sub(r".*DECLARE_.*", "", data)
        data = re.sub(r"(?:(?<=\(|,).*?)(\s*(?:\bclass\b|\bstruct\b|\benum\b)\s*)(?=.*\))", "", data)
        data = re.sub(r"\w*_API\s*", "", data)
        data = re.sub(r"friend.*;", "", data)

        with open("../Data/test_subtracted.h", "w") as file:
            file.write(data)

        header = GLibCppHeaderParser(data, argType="string", encoding="utf-8", delegates=parsed_delegates, settings=settings)

        generated_data = "\n"
        generated_data = cls.parse_pragmas(generated_data, header)
        generated_data += "\n"
        generated_data = cls.parse_includes(generated_data, header)
        generated_data += "\n" * 2
        generated_data = cls.parse_forward_declarations(generated_data, data)
        generated_data += "\n" * 2
        generated_data += GLibDelegateParser.crete_multicast_delegates(parsed_delegates, settings)
        generated_data += "\n"
        generated_data = cls.parse_enums(generated_data, header)
        generated_data = cls.parse_classes(generated_data, header)

        generated_data = header.fixup_names(generated_data)


        with open("../Data/generated.h", "w+", encoding="utf-8") as file:
            file.write(generated_data)

        # with open(r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.json", "w+", encoding="utf-8") as f:
        #     f.write(header.toJSON())

    @classmethod
    def parse_pragmas(cls, generated_data: str, header: GLibCppHeaderParser):
        for pragma in header.pragmas:
            generated_data += "#pragma " + pragma + "\n"
        return generated_data

    @classmethod
    def parse_forward_declarations(cls, generated_data: str, data: str):
        forward_declarations = re.findall(r"^\s*(?:class|struct|enum)\s\w+;", data, re.MULTILINE)

        for forward_declaration in forward_declarations:
            generated_data += forward_declaration.strip().replace("\n", "") + "\n"

        return generated_data

    @classmethod
    def parse_includes(cls, generated_data: str, header: GLibCppHeaderParser):
        for include in header.includes:
            generated_data += "#include " + include + "\n"
        return generated_data

    @classmethod
    def parse_classes(cls, generated_data: str, header: GLibCppHeaderParser):
        for parsed_class, content in header.classes.items():
            if not content["declaration_method"] == "struct":
                continue

            generated_data += GLibStructParser.parse(content)
            generated_data += "\n"

        for parsed_class, content in header.classes.items():
            if not content["declaration_method"] == "class":
                continue

            generated_data += GLibClassParser.parse(content)
            generated_data += "\n"

        return generated_data

    @classmethod
    def parse_enums(cls, generated_data: str, header: GLibCppHeaderParser):
        for enum in header.enums:
            generated_data += GLibEnumParser.parse(enum)
            generated_data += "\n"

        for parsed_class, content in header.classes.items():
            enums = content.get("enums")
            for access_specifier, enums in enums.items():
                for enum in enums:
                    generated_data += GLibEnumParser.parse(enum)
                    generated_data += "\n"

        return generated_data


GLibWrapperGenerator.parse(
    r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.h",
    settings=
    {
        "prefix_override": "GLib",
        "parent_category": "GLib",
        "sort_delegates": True,
    }
)

