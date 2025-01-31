# Программа для взаимодействия через сокеты

Этот проект состоит из двух программ (клиент и сервер) и библиотеки, которые взаимодействуют через сокеты. Клиент принимает строки от пользователя, обрабатывает их с помощью библиотеки и отправляет результаты на сервер. Сервер проверяет данные и выводит сообщения о получении.

---

## Требования

Для работы с проектом необходимы следующие компоненты:

- Компилятор `g++` (поддерживающий C++17).
- `CMake` (версия 3.10 или выше).
- Операционная система: Linux или Windows (с поддержкой WSL для Linux-среды).

---

## Структура проекта

```
test_task/
├── CMakeLists.txt          # Корневой файл CMake
├── lib/                    # Динамическая библиотека
│   ├── CMakeLists.txt
│   ├── library.cpp
│   └── library.h
├── program1/               # Программа №1 (клиент)
│   ├── CMakeLists.txt
│   └── main.cpp
├── program2/               # Программа №2 (сервер)
│   ├── CMakeLists.txt
│   └── main.cpp
└── README.md               # Этот файл
```

---

## Сборка и запуск

### Сборка проекта

1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/Helbushek/test_task
   cd test_task
   ```

2. Создайте папку для сборки:
   ```bash
   mkdir build
   cd build
   ```

3. Соберите проект с помощью CMake:
   ```bash
   cmake ..
   make
   ```

---

### Запуск программ

1. Запустите сервер (Программа №2):
   ```bash
   ./program2/program2
   ```

2. Запустите клиент (Программа №1):
   ```bash
   ./program1/program1
   ```

---

## Описание программ

### Программа №1 (Клиент)

- Принимает строки от пользователя.
- Проверяет, что строка состоит только из цифр и не превышает 64 символа.
- Обрабатывает строку с помощью функции `function1` из библиотеки.
- Отправляет результат на сервер через сокет.
- При потере соединения автоматически переподключается к серверу.

---

### Программа №2 (Сервер)

- Ожидает подключения клиента.
- Принимает данные от клиента.
- Проверяет данные с помощью функции `function3` из библиотеки.
- Выводит сообщение о получении данных или об ошибке.

---

## Библиотека

Библиотека содержит три функции:

1. **`function1`**:
   - Сортирует строку по убыванию.
   - Заменяет все четные элементы на `"KB"`.
   - Не возвращает значение.

2. **`function2`**:
   - Вычисляет сумму всех численных значений в строке.
   - Возвращает целое число.

3. **`function3`**:
   - Проверяет, что длина строки больше 2 символов и кратна 32.
   - Возвращает `true`, если условие выполняется, иначе `false`.

---

## Автор

Горевой Александр Олегович
