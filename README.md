# SparseFile
Консольная утилита на языке Си для создания разрежённого файла

## Установка
```bash
git clone https://github.com/AtkishkinVlad/SparseFile.git
cd SparseFile
make
```

## Запуск тестов
```bash
source runme.sh
```

## Результаты тестов
```
fileA.dat -- 8200 blocks with 512 bytes in block
fileB.dat -- 24 blocks with 512 bytes in block
fileC.dat -- 24 blocks with 512 bytes in block
fileD.dat -- 32 blocks with 512 bytes in block
fileA.dat.gz -- 16 blocks with 512 bytes in block
fileB.dat.gz -- 16 blocks with 512 bytes in block
e0d587fa8e49f37c987bbf20241d74e6  fileA.dat
e0d587fa8e49f37c987bbf20241d74e6  fileB.dat
e0d587fa8e49f37c987bbf20241d74e6  fileC.dat
e0d587fa8e49f37c987bbf20241d74e6  fileD.dat
```

Последние 4 строки нужны для подтверждения идентичности содержимого файлов (идентичность устанавливал с помощью утилиты md5sum)
