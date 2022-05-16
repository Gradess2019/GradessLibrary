import sys


class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def __print_unreal__(message, color=None):
    import unreal

    if color == Colors.WARNING:
        unreal.log_warning(message)
    elif color == Colors.FAIL:
        unreal.log_error(message)
    else:
        unreal.log(message)


def __print_console__(message, color=None):
    if color:
        print(color + message + Colors.ENDC)
    else:
        print(message)


if "unreal" in sys.modules:
    print_message = __print_unreal__
else:
    print_message = __print_console__


def log(message):
    print_message(message)


def success(message):
    print_message(message, Colors.OKGREEN)


def warn(message):
    print_message(message, Colors.WARNING)


def error(message):
    print_message(message, Colors.FAIL)

