# 說明
## 檔案
- data.txt: 歷年來大樂透的資料，使用者需自行更新
- lita.cpp: 原始程式碼
- Makefile: 編譯檔
- result.txt: 分析結果


- lita.exe: 編譯過後的執行檔


## 編譯
### Linux
在相同路徑下執行 `make`

## 執行
### Linux
./lita.exe
./lita.exe 200 18 6 8
./lita.exe [cycle] [sum3] [sum4] [thread_num]

- cycle: 與資料中最新的幾筆做比對，預設為 `200`
- sum3: 成功比對三個相同數字，超過幾筆則挑選出，預設 18 (成功比對四筆也算)
- sum4: 成功比對四個相同數字，超過幾筆則挑選出，預設 6
- thread_num: 平行化數量，預設為 `8`
