from CppHeaderParser import *


class GLibFunctionParser:
    DECLARATION_EXTRA_SPACES = r"(?:(?<=\()\s+)|(?:\s+((?=&|;|,|\)|\())+)"

    @classmethod
    def parse(cls, data):
        parsed_function = "\t" + data["doxygen"].replace("\n", "\n\t") + "\n" if data.get("doxygen") else ""
        parsed_function += "\tUFUNCTION({callable_type}, Category = \"{category}\")\n".format_map({
            "callable_type": "BlueprintCallable" if data["rtnType"] == "void" else "BlueprintPure",
            "category": data["category"] if data.get("category") else "GLib"
        })

        declaration = data["debug"]
        declaration = re.sub(cls.DECLARATION_EXTRA_SPACES, "", declaration)
        parsed_function += "\t" + declaration + "\n"

        return parsed_function


class GLibWrapperGenerator:
    @classmethod
    def parse(cls, path: str):
        header = CppHeaderParser.CppHeader(path)

        classes = header.classes

        ufunction = "UFUNCTION(BlueprintCallable, Category = \"GLib|PluginUtils\")"

        generated_class = ""

        for parsed_class, content in classes.items():
            all_methods = content["methods"]
            generated_class += "class " + parsed_class + "\n{\n"
            generated_class += "\tGENERATED_BODY()\n\n"

            for access_modifier, methods in all_methods.items():
                if len(methods) == 0:
                    continue

                generated_class += access_modifier + ":\n"
                for method in methods:
                    generated_class += GLibFunctionParser.parse(method) + "\n"

        generated_class += "};\n"
        with open("../Data/generated.h", "w+", encoding="utf-8") as f:
            f.write(generated_class)


GLibWrapperGenerator.parse(r"D:\Projects\UE\5\Spacegod\Plugins\GradessLibrary\Content\Python\Data\test.h")

