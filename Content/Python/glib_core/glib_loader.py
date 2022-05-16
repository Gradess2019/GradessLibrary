import importlib
import importlib.util
import json
import os
import re
import sys
import types
import unreal
from glib_core.glib_log import log, warn, error


class GLibLoaderBaseRules:
    def __init__(self):
        self.blacklist = [os.path.basename(__file__), "init_unreal.py", "__init__.py", "glib_load.py"]

    def is_valid_file(self, file_path):
        python_file = file_path.endswith(".py")
        blacklist_file = os.path.basename(file_path) in self.blacklist

        return python_file and not blacklist_file


class GLibLoaderInitFileRules(GLibLoaderBaseRules):
    def is_valid_file(self, file_path):
        return file_path.endswith("__init__.py") and not os.path.dirname(file_path).endswith("Python")


# noinspection PyBroadException
class GLibLoader:

    @classmethod
    def load_modules(cls, root):
        if not os.path.exists(root):
            error(f"Python folder not found in project content folder: {root}")
            return

        module_order_file = unreal.Paths.combine([root, "glib_module_order.json"])
        if not os.path.exists(module_order_file):
            error(f"glib_module_order.json not found in Python folder: {module_order_file}")
            return

        cls.add_packages_to_environment(root)
        cls.add_files_to_environment(root)

        # Load modules
        module_order = cls.get_ordered_modules(module_order_file)

        for priority, modules in module_order.items():
            for module in modules:
                cls.load_module_by_name(module)

    @classmethod
    def load_project_modules(cls):
        content_dir = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_content_dir())
        root = unreal.Paths.combine([content_dir, "Python"])

        cls.load_modules(root)

    @classmethod
    def load_plugin_modules(cls, plugin_name):
        plugins_dir = unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_plugins_dir())
        root = unreal.Paths.combine([plugins_dir, plugin_name, "Content", "Python"])

        cls.load_modules(root)

    @classmethod
    def collect_init_files(cls, root: str):
        return cls.__collect_files__(root, GLibLoaderInitFileRules())

    @classmethod
    def collect_python_files(cls, root: str):
        return cls.__collect_files__(root)

    @classmethod
    def __collect_files__(cls, root: str, rules=GLibLoaderBaseRules()):
        collected_files = []
        for root, directories, files in os.walk(root):
            for file in files:
                file_path = unreal.Paths.normalize_filename(unreal.Paths.combine([root, file]))
                if rules.is_valid_file(file_path):
                    collected_files.append(file_path)

        return collected_files

    @classmethod
    def load_module_by_name(cls, module_name):
        try:
            importlib.reload(sys.modules.get(module_name))
            log(f"Module loaded: {module_name}")
        except Exception as e:
            error(f"Couldn't load {module_name}: {e.args[0] if len(e.args) > 0 else e}")

    @classmethod
    def add_package_to_environment(cls, file_path: str):
        module_name = cls.get_module_name(file_path)

        if not module_name:
            warn(f"Couldn't find module name for package: {file_path}")
            return None

        file_path = unreal.Paths.normalize_filename(file_path)

        module = types.ModuleType(module_name)

        module.__path__ = os.path.dirname(file_path)

        sys.modules[module_name] = module

        importlib.import_module(module_name)
        importlib.reload(sys.modules[module_name])

        log(f"Package {module_name} was added to environment: {file_path}")

        return module_name

    @classmethod
    def add_packages_to_environment(cls, root: str):
        init_files = cls.collect_init_files(root)
        for init_file in init_files:
            cls.add_package_to_environment(init_file)

    @classmethod
    def add_file_to_environment(cls, file_path: str):
        module_name = cls.get_module_name(file_path)

        if not module_name:
            warn(f"Couldn't find module name for {file_path}")
            return None

        file_path = unreal.Paths.normalize_filename(file_path)

        module = types.ModuleType(module_name)

        if file_path.endswith("__init__.py"):
            module.__path__ = os.path.dirname(file_path)
        else:
            module.__file__ = file_path

        sys.modules[module_name] = module
        log(f"Module {module_name} was added to environment: {file_path}")

        return module_name

    @classmethod
    def add_files_to_environment(cls, root: str):
        files = cls.collect_python_files(root)
        for file in files:
            cls.add_file_to_environment(file)

    @classmethod
    def get_module_name(cls, path: str, root="Python"):
        if path.endswith(root) or path.endswith("__init__.py") and os.path.dirname(path).endswith(root):
            error(f"{path} is not a valid module path")
            return None

        if path.endswith("__init__.py"):
            module_name = os.path.basename(os.path.dirname(path))
            return module_name

        relative_path = re.sub(rf".*{root}[\\/]*", "", path)
        module_path = re.sub(r"\..*$", "", relative_path)
        module_name = re.sub(r"[/\\]", ".", module_path)

        return module_name

    @classmethod
    def get_ordered_modules(cls, module_order_path: str):
        if not os.path.exists(module_order_path):
            error(f"Module order file not found: {module_order_path}")
            return []

        with open(module_order_path, "r") as json_file:
            module_order = json.load(json_file)

        ordered_modules = dict()
        for module_name, priority in module_order.items():
            if priority not in ordered_modules:
                ordered_modules[priority] = []

            ordered_modules[priority].append(module_name)

        return ordered_modules

