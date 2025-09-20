# CS2 Weapon Control

Автоматизированная система компенсации отдачи и контр-страфирования для Counter-Strike 2.

## Сборка проекта

### 1. Установка Chocolatey

Откройте PowerShell от имени администратора и выполните:

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

### 2. Установка необходимых инструментов

Установите CMake и Visual Studio Build Tools через Chocolatey:

```powershell
choco install cmake -y
choco install visualstudio2022buildtools -y
choco install visualstudio2022-workload-vctools -y
```

### 3. Сборка проекта

Запустите `build.bat`:

```cmd
build.bat
```

Или выполните команды вручную:

```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 4. Запуск

После сборки исполняемый файл будет находиться в `build/Release/cs2_weapon_control.exe`.
для указания сенсы используйте

`cs2_weapon_control.exe --sens 3.10 (вала сенса 3.10 стандартная (моя))`
## Управление

- **F8** - Включить/выключить скрипт
- **F9** - Переключить оружие
- **F11** - Отключить оружие
- **F6** - Включить/выключить контр-страфирование
- **F12** - Выход

