# MyHFT
Platform: Ubuntu

## Что реализовано
- пока что ничего

## Что не реализовано
- логгирование

## Зависимости
- curl - нужно поставить самостоятельно
- rapidjson - скачается автоматически
- gtest - скачается автоматически

## How to
git clone https://github.com/TarakanoLov/MyHFT
cd MyHFT
mkdir Release && cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./hft.exe
./tests.exe - для тестов