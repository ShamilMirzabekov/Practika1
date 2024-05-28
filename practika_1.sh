#!/bin/bash
# Функция для вывода справки
function help_message {
    echo "Использование $(basename "$0") [Доступные аргументы]"
    echo "Доступные аргументы:"
    echo "-u, --users: Выводит перечень пользователей и их домашних директорий, отсортированных по алфавиту."
    echo "-p, --processes: Выводит перечень запущенных процессов, отсортированных по их идентификатору."
    echo "-h, --help: Выводит справку с перечнем и описанием аргументов и останавливает работу."
    echo "-l PATH, --log PATH: Замещает вывод на экран выводом в файл по заданному пути PATH."
    echo "-e PATH, --errors PATH: Замещает вывод ошибок из потока stderr в файл по заданному пути PATH."
}
# Функция для вывода списка пользователей и их домашних директорий
function print_users {
    getent passwd | awk -F ':' '{print "Пользователь:", $1, "Домашняя директория:", $6}' | sort
}
# Функция для вывода списка всех запущенных процессов, отсортированных по их pid
function print_processes {
    ps -eo pid,args --sort pid
}
# Функция для перенаправления стандартного вывода в файл по указанному пути
function redirect_stdout {
    local path=$1
    shift
    "$@" > "$path"
}
# Функция для перенаправления ошибок в файл
function redirect_stderr {
    local path=$1
    shift
    "$@" 2> "$path"
}

# Обработка аргументов командной строки
while getopts ":u:p:hl:e:-:" opt; do
    case $opt in
        u)
            print_users
            exit 0
            ;;
        p)
            print_processes
            exit 0
            ;;
        h)
            help_message
            exit 0
            ;;
        l)
            redirect_stdout "$OPTARG" "${@:OPTIND}"
            exit 0
            ;;
        e)
            redirect_stderr "$OPTARG" "${@:OPTIND}"
            exit 0
            ;;
        -)
            # Обработка длинных параметров
            case $OPTARG in
                users )
                    print_users
                    exit 0
                    ;;
                processes )
                    print_processes
                    exit 0
                    ;;
                help )
                    help_message
                    exit 0
                    ;;
                log )
                    redirect_stdout "${!OPTIND}" "${@:OPTIND+1}"
                    exit 0
                    ;;
                errors )
                    redirect_stderr "${!OPTIND}" "${@:OPTIND+1}"
                    exit 0
                    ;;
            esac
            ;;
        ?)
            echo "Неверный аргумент: -$OPTARG" >&2
            help_message
            exit 1
            ;;
        :)
            echo "Требуется аргумент для параметра: -$OPTARG" >&2
            help_message
            exit 1
            ;;
    esac
done

# Если были переданы пустые аргументы
if [[ $# -eq 0 ]]; then
    help_message
    exit 0
fi
