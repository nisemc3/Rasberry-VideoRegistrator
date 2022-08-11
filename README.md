# Rasberry  
Rasberry PI 4b - Video server  

Для успешной сборки необходим opencv версии 4.5.0.  
Для запуска на новой системе rasberry необходимо заново собирать opencv  

# Запуск  
bash-скрипт  

#!/bin/bash  
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:*путь до opencv 4.5.0*/lib  
./*название программы*  

# Test-coverage  
запуск bash-скрипта в папке билда  
создаст папки с постфиксом *_coverage  

#!/bin/bash  
make storagemanager_test  
make videocapturemanager_test  
make threadpool_test  
make connector_test  

