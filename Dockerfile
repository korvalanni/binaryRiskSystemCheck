# Используем официальный образ с GCC
FROM gcc:latest

# Устанавливаем рабочую директорию
WORKDIR /usr/src/app

# Устанавливаем необходимые инструменты
RUN apt update && \
    apt install -y cmake && \
    rm -rf /var/lib/apt/lists/*

# Копируем исходный код в контейнер
COPY . .

# Создаем директорию для сборки и переходим в нее
RUN mkdir -p build
WORKDIR /usr/src/app/build

# Запускаем процесс сборки
RUN cmake .. && make

# Устанавливаем команду для запуска приложения
CMD ["./your_executable_name"]
