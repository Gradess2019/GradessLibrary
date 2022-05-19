from CppHeaderParser import *


class GLibClassParser:
    @classmethod
    def parse(cls, data):
        result = data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "UCLASS(BlueprintType, Blueprintable)\n"
        result += "class " + data["name"] + " : public UObject\n"
        result += "{\n"
        result += "\tGENERATED_BODY()\n"
        result += "\n"

        for access_modifier, properties in data["properties"].items():
            if len(properties) == 0:
                continue

            result += access_modifier + ":\n"
            for class_property in properties:
                result += GLibPropertyParser.parse(class_property) + "\n"

        for access_modifier, methods in data["methods"].items():
            if len(methods) == 0:
                continue

            result += access_modifier + ":\n"
            for method in methods:
                result += GLibFunctionParser.parse(method) + "\n"

        result += "};\n"

        return result


class GLibPropertyParser:
    @classmethod
    def parse(cls, data):
        result = "\t" + data["doxygen"] + "\n" if data.get("doxygen") else ""
        result += "\t" + "UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = \"{category}\")" + "\n"
        category = data["category"] if data.get("category") else "GLib"

        if data.get("property_of_class"):
            category += f"|{data.get('property_of_class')}"
            
        result = result.format_map({"category": category})
        result += "\t" + data["type"] + " " + data["name"] + ";\n"

        return result


class GLibFunctionParser:
    DECLARATION_EXTRA_SPACES = r"(?:(?<=\()\s+)|(?:\s+((?=&|;|,|\)|\())+)"

    @classmethod
    def parse(cls, data):
        result = "\t" + data["doxygen"].replace("\n", "\n\t") + "\n" if data.get("doxygen") else ""
        result += "\tUFUNCTION({callable_type}, Category = \"{category}\")\n".format_map({
            "callable_type": "BlueprintCallable" if data["rtnType"] == "void" else "BlueprintPure",
            "category": data["category"] if data.get("category") else "GLib"
        })

        declaration = data["debug"]
        declaration = re.sub(cls.DECLARATION_EXTRA_SPACES, "", declaration)
        result += "\t" + declaration + "\n"

        return result


class GLibWrapperGenerator:
    @classmethod
    def parse(cls, path: str):
        header = CppHeaderParser.CppHeader(path)

        generated_data = ""

        for parsed_class, content in header.classes.items():
            generated_data += GLibClassParser.parse(content)
            generated_data += "\n"

        with open("../Data/generated.h", "w+", encoding="utf-8") as f:
            f.write(generated_data)


# result = CppHeaderParser.CppHeader(r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.h").toJSON()
# with open(r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.json", "w+", encoding="utf-8") as f:
#     f.write(result)

GLibWrapperGenerator.parse(r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.h")

