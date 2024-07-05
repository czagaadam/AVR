# HOW TO USE GPIO LIBRARY

1. Add SPI base library to your workspace (solution in Micropchip studio)
![image](https://github.com/czagaadam/AVR/assets/168843740/c52632ef-8acf-412d-895a-12b3651c5654)

2. Add library to your main startup project

![image](https://github.com/czagaadam/AVR/assets/168843740/db3f74a5-1c0a-4125-9a89-3bf22c0a0158)

![image](https://github.com/czagaadam/AVR/assets/168843740/4d477868-c708-4e55-92f7-c07603ef2638)


3. Open the properties of your startup project and set the include path of the GPIO library in the Toolchain/C++ compiler section
![image](https://github.com/czagaadam/AVR/assets/168843740/624e0b33-f9f0-4663-bd63-75220356cc85)

4. If you add libraries to your solution there's no need to add libraries Toolchain/GNU linker/Libraries section

5. Include SPI library to the main.cpp file

![image](https://github.com/czagaadam/AVR/assets/168843740/c3fe94bf-009e-4362-a322-fc03363c25af)

