import re

from CppHeaderParser import *


class GLibCppHeaderParser(CppHeader):
    def __init__(self, headerFileName, argType="file", encoding=None, delegates=None, **kwargs):
        self.delegates = delegates
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

    def _evaluate_property_stack(self, clearStack=True, addToVar=None):
        current_class = self.classes[self.curClass]
        properties = current_class["properties"][self.curAccessSpecifier]
        before_count = len(properties)
        super()._evaluate_property_stack(clearStack=clearStack, addToVar=addToVar)

        if len(properties) == 0 or len(properties) == before_count:
            return

        new_property = properties[-1]
        new_property["delegate"] = new_property["type"] in self.delegates


class GLibBaseParser:
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
                result += GLibPropertyParser.parse(class_property) + "\n"

        return result

    @classmethod
    def parse_methods(cls, data):
        result = ""
        for access_modifier, methods in data["methods"].items():
            if len(methods) == 0:
                continue

            result += access_modifier + ":\n"
            for class_method in methods:
                result += GLibFunctionParser.parse(class_method) + "\n"

        return result


class GLibClassParser(GLibBaseParser):
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "UCLASS(BlueprintType, Blueprintable)\n"
        result += "class " + data["name"] + " : public UObject\n"
        result += "{\n"
        result += "\tGENERATED_BODY()\n"
        result += "\n"

        result += super().parse(data)

        return result


class GLibStructParser(GLibBaseParser):
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "USTRUCT(BlueprintType, Blueprintable)\n"
        result += "struct " + data["name"] + "\n"
        result += "{\n"
        result += "\tGENERATED_BODY()\n"
        result += "\n"

        result += super().parse(data)

        return result


class GLibEnumParser():
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "UENUM(BlueprintType)\n"
        result += "enum " + ("class " if data["isclass"] else "") + data["name"] + " : uint8" + "\n"
        result += "{\n"

        result += GLibEnumValueParser.parse_values(data)

        result += "};\n"
        return result


class GLibMemberParser:
    @classmethod
    def get_doxygen(cls, data):
        return "\t" + data["doxygen"].replace("\n", "\n\t") + "\n" if data.get("doxygen") else ""


class GLibPropertyParser(GLibMemberParser):
    @classmethod
    def parse(cls, data):
        result = cls.get_doxygen(data)

        if data["delegate"]:
            result += "\tUPROPERTY(BlueprintAssignable, Category = \"{category}\")" + "\n"
        else:
            result += "\t" + "UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = \"{category}\")" + "\n"

        category = data["category"] if data.get("category") else "GLib"

        if data.get("property_of_class"):
            category += f"|{data.get('property_of_class')}"
            
        result = result.format_map({"category": category})
        result += "\t" + data["type"].replace(" ", "") + " " + data["name"] + ";\n"

        return result


class GLibFunctionParser(GLibMemberParser):
    DECLARATION_EXTRA_SPACES = r"(?:(?<=\()\s+)|(?:\s+((?=&|;|,|\)|\())+)"

    @classmethod
    def parse(cls, data):
        result = cls.get_doxygen(data)

        if not data["constructor"]:
            result += "\tUFUNCTION({callable_type}, Category = \"{category}\")\n".format_map({
                "callable_type": "BlueprintPure" if data["rtnType"] != "void" and data["const"] else "BlueprintCallable",
                "category": data["category"] if data.get("category") else "GLib"
            })

        declaration = data["debug"]
        declaration = re.sub(cls.DECLARATION_EXTRA_SPACES, "", declaration)
        declaration = re.sub(r"\s*(?:{|:).*$", ";", declaration)
        result += "\t" + declaration + "\n"

        return result


class GLibEnumValueParser(GLibMemberParser):
    @classmethod
    def parse(cls, data):
        result = cls.get_doxygen(data)
        result += "\t" + data["name"] + " = " + str(data["value"])

        return result

    @classmethod
    def parse_values(cls, data):
        result = ""
        for index, value in enumerate(data["values"]):
            result += GLibEnumValueParser.parse(value)

            if not data.get("isclass"):
                result += " UMETA(DisplayName = \"" + value["name"] + "\")"

            result += ",\n" if index < len(data["values"]) - 1 else "\n"

        return result


class GLibDelegateParser:
    VOID_DELEGATE = r"DECLARE.*DELEGATE(?!.*RetVal).*"
    SPARSE_DELEGATE = r"DECLARE.*SPARSE.*DELEGATE.*"
    SPARSE_DELEGATE_PARAMS = r"(?:(?<=\().*?,.*?,.*?,)(.*(?=\)))"
    DELEGATE_NAME = r"(?<=\()\w*"
    DELEGATE_PARAMS = r"(?<=,).*(?=\))"
    DELEGATE_PARAMS_NUMBER = r"(?<=_)[a-zA-Z]+(?=\()"

    @classmethod
    def parse(cls, raw_data):
        delegates = re.findall(cls.VOID_DELEGATE, raw_data)

        parsed_delegates = cls.parse_delegates(delegates)
        parsed_delegates = cls.parse_params(parsed_delegates)

        return parsed_delegates

    @classmethod
    def parse_delegates(cls, delegates):
        parsed_delegates = dict()
        for delegate in delegates:
            delegate_name = re.search(cls.DELEGATE_NAME, delegate.replace(" ", "")).group(0)
            parsed_delegates[delegate_name] = dict()
            parsed_delegates[delegate_name]["delegate"] = delegate
            parsed_delegates[delegate_name]["sparse"] = re.search(cls.SPARSE_DELEGATE, delegate) is not None
        return parsed_delegates

    @classmethod
    def parse_params(cls, parsed_delegates):
        for delegate_name, data in parsed_delegates.items():
            if data["sparse"]:
                delegate_params_match = re.search(cls.SPARSE_DELEGATE_PARAMS, data["delegate"])
                params = delegate_params_match.group(1) if delegate_params_match else ""
            else:
                delegate_params_match = re.search(cls.DELEGATE_PARAMS, data["delegate"])
                params = delegate_params_match.group(0) if delegate_params_match else ""

            data["params"] = params.strip()

        return parsed_delegates

    @classmethod
    def crete_multicast_delegates(cls, parsed_delegates):
        result = ""
        for delegate_name, data in parsed_delegates.items():
            new_delegate = "DECLARE_DYNAMIC_MULTICAST_DELEGATE"
            if data["params"] != "":
                params_number = re.search(cls.DELEGATE_PARAMS_NUMBER, data["delegate"]).group(0)
                new_delegate += f"_{params_number}"

            new_delegate += f"({delegate_name}" + (", " + data["params"] if data["params"] != "" else "") + ");\n"
            result += new_delegate
        return result


class GLibWrapperGenerator:
    @classmethod
    def parse(cls, path: str):
        if not os.path.exists(path):
            raise Exception("Path does not exist")

        with open(path, "r") as file:
            data = file.read()

        parsed_delegates = GLibDelegateParser.parse(data)

        data = re.sub(r"\n.*(?:UCLASS|UENUM|UFUNCTION|UPROPERTY|USTRUCT|GENERATED.*BODY|UE_DEPRECATED|UMETA)(?:\([\s\S]*?\)(?:,[\s\S]*?\))*)(?:\)*)", "", data)
        data = re.sub(r"(?:PRAGMA_ENABLE_DEPRECATION_WARNINGS|PRAGMA_DISABLE_DEPRECATION_WARNINGS).*", "", data)
        data = re.sub(r".*DECLARE_.*", "", data)
        data = re.sub(r"(?:(?<=\(|,).*?)(\s*(?:\bclass\b|\bstruct\b|\benum\b)\s*)(?=.*\))", "", data)
        data = re.sub(r"\w*_API\s*", "", data)

        with open("../Data/test_subtracted.h", "w") as file:
            file.write(data)

        header = GLibCppHeaderParser(data, argType="string", encoding="utf-8", delegates=parsed_delegates)

        generated_data = "\n"
        generated_data = cls.parse_pragmas(generated_data, header)
        generated_data += "\n"
        generated_data = cls.parse_includes(generated_data, header)
        generated_data += "\n\n"
        generated_data += GLibDelegateParser.crete_multicast_delegates(parsed_delegates)
        generated_data += "\n"
        generated_data = cls.parse_classes(generated_data, header)
        generated_data = cls.parse_enums(generated_data, header)

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
    def parse_includes(cls, generated_data: str, header: GLibCppHeaderParser):
        for include in header.includes:
            generated_data += "#include " + include + "\n"
        return generated_data

    @classmethod
    def parse_classes(cls, generated_data: str, header: GLibCppHeaderParser):
        for parsed_class, content in header.classes.items():
            if content["declaration_method"] == "class":
                generated_data += GLibClassParser.parse(content)
            elif content["declaration_method"] == "struct":
                generated_data += GLibStructParser.parse(content)

            generated_data += "\n"
        return generated_data

    @classmethod
    def parse_enums(cls, generated_data: str, header: GLibCppHeaderParser):
        for enum in header.enums:
            generated_data += GLibEnumParser.parse(enum)
            generated_data += "\n"
        return generated_data


GLibWrapperGenerator.parse(r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.h")

