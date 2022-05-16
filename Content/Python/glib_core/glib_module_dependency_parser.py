import json
import os.path
import re
from collections import defaultdict


class DependencyNode:
    """Represents a graph node"""
    priority = -1
    module_name = ""
    dependencies = []

    def __str__(self):
        return f"{self.module_name}, {self.priority} => {self.dependencies}"


class NodeEncoder(json.JSONEncoder):
    """Custom JSON encoder for DependencyNode"""
    def default(self, o):
        if isinstance(o, DependencyNode):
            return o.priority

        return json.JSONEncoder.default(self, o)


class ModulesContainer:
    """Container that let you add and get nodes"""
    def __init__(self):
        self.modules = {}

    def add_module_node(self, node: DependencyNode):
        """Add module dependency node

        :param node: dependency node to add
        """
        self.modules[node.module_name] = node

    def get_module_nodes(self):
        """Get all module dependency nodes

        :return: list of dependency nodes
        """
        return self.modules.values()

    def get_module_node(self, module_name):
        """Get module dependency node by name

        :param module_name: module name to get dependency node associated with this name
        :return: found dependency node
        """
        return self.modules.get(module_name)


class ModuleGraph:
    """Graph that represents dependencies between modules"""
    def __init__(self, modules_container: ModulesContainer):
        self.graph = defaultdict(list)
        self.modules_container = modules_container

    def add_dependency(self, child_module: DependencyNode, parent_module: DependencyNode):
        """Add child module dependency

        :param child_module: child module name
        :param parent_module: parent module name
        """
        self.graph[child_module.module_name].append(parent_module.module_name)

    def __set_priority_recursive__(self, module: DependencyNode, visited: set):
        """Set module priority using Depth-First Search

        :param module: module to set priority
        :param visited: set of visited nodes
        """
        visited.add(module.module_name)

        priority = max(module.priority, 0)

        for dependency_name in self.graph[module.module_name]:
            dependency_node = self.modules_container.get_module_node(dependency_name)
            if dependency_name not in visited:
                self.__set_priority_recursive__(dependency_node, visited)

            dependency_priority = dependency_node.priority
            priority = max(priority, dependency_priority + 1)

        module.priority = priority

    def set_module_priorities(self):
        """Set module priorities"""
        visited = set()

        for node in self.modules_container.get_module_nodes():
            self.__set_priority_recursive__(node, visited)


class GLibModuleDependencyParser:
    """Tool that search dependencies and set load order for the python script"""
    from glib_core.glib_loader import GLibLoader, GLibLoaderBaseRules

    IMPORT_PATTERN = r"(?<=^\s{4}from\s).+(?= import)|(?<=^from\s).+(?= import)"

    @classmethod
    def create_module_priorities_json(cls, root: str, json_path: str):
        """Create module priorities json that is used by reloader

        :param root: start directory to collect python files and parse them all
        :param json_path: path to save json
        """
        modules_container = cls.parse_directory(root)

        graph = cls.init_graph(modules_container)
        graph.set_module_priorities()

        cls.save_json(json_path, modules_container.modules, encoder=NodeEncoder)

    @classmethod
    def parse_directory(cls, directory: str):
        """Get all python scripts in directory (including subdirectories) and parse each of them into DependencyNode

        :param directory: directory to parse
        :return: modules container with created dependency nodes
        """
        modules_container = ModulesContainer()

        for root, directories, files in os.walk(directory):
            for item in files:
                path = os.path.join(root, item)
                if not cls.GLibLoaderBaseRules().is_valid_file(path):
                    continue

                new_node = cls.parse_file(path)
                modules_container.add_module_node(new_node)

        return modules_container

    @classmethod
    def parse_file(cls, path: str):
        """Create DependencyNode from python file

        :param path: path to file
        :return: created DependencyNode object
        """
        with open(path, "r") as python_file:
            file_data = python_file.read()

            node = DependencyNode()
            node.module_name = cls.GLibLoader.get_module_name(path)
            node.dependencies = cls.extract_dependencies_from_file(file_data)

        return node

    @classmethod
    def extract_dependencies_from_file(cls, file_data: str):
        """Find import directives and extract dependencies

        :param file_data: file data like text
        :return: script dependencies
        """
        return re.findall(cls.IMPORT_PATTERN, file_data, re.M)

    @classmethod
    def init_graph(cls, modules_container: ModulesContainer):
        """Initialize ModuleGraph that represents dependencies between modules

        :param modules_container: initialized ModulesContainer object
        :return: created ModuleGraph
        """
        graph = ModuleGraph(modules_container)
        for node in modules_container.get_module_nodes():
            dependencies = node.dependencies
            valid_dependencies = []
            for dependency in dependencies:
                dependency_module = modules_container.get_module_node(dependency)
                if not dependency_module:
                    continue

                valid_dependencies.append(dependency_module)
                graph.add_dependency(node, dependency_module)

            node.dependencies = valid_dependencies

        return graph

    @classmethod
    def save_json(cls, path: str, obj, encoder=json.JSONEncoder):
        """Save object to json

        :param path: path to json
        :param obj: object to save
        :param encoder: custom JSON encoder
        """
        with open(path, 'w') as json_file:
            json.dump(obj, json_file, sort_keys=True, indent=4, cls=encoder)
