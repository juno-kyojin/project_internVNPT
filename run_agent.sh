#!/bin/bash

# Đường dẫn tới chương trình agent
AGENT_PATH="./example_agent_server/obj/agent"

# Đường dẫn tới tệp log
LOG_FILE="./logs/agent_$(date +%Y%m%d_%H%M%S).log"

# Kiểm tra xem chương trình agent có tồn tại không
if [ ! -f "$AGENT_PATH" ]; then
    echo "Lỗi: Chương trình agent không tồn tại tại $AGENT_PATH"
    echo "Vui lòng biên dịch chương trình agent trước khi chạy."
    exit 1
fi

# Chạy chương trình agent và ghi log
echo "Đang chạy chương trình agent... Ghi log vào $LOG_FILE"
stdbuf -oL "$AGENT_PATH" 2>&1 | tee "$LOG_FILE" | grep --line-buffered -E "Discovery Response thành công|Kết thúc chương trình"
