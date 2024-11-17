#!/bin/bash

# Đường dẫn tới chương trình server
SERVER_PATH="./example_agent_server/obj/server"

# Đường dẫn tới tệp log
LOG_FILE="./logs/server_$(date +%Y%m%d_%H%M%S).log"

# Kiểm tra xem chương trình server có tồn tại không
if [ ! -f "$SERVER_PATH" ]; then
    echo "Lỗi: Chương trình server không tồn tại tại $SERVER_PATH"
    echo "Vui lòng biên dịch chương trình server trước khi chạy."
    exit 1
fi

# Chạy chương trình server và ghi log
echo "Đang chạy chương trình server... Ghi log vào $LOG_FILE"
stdbuf -oL "$SERVER_PATH" 2>&1 | tee "$LOG_FILE" | grep --line-buffered -E "Discovery Response thành công|Kết thúc chương trình"
