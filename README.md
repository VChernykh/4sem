# 4sem
The program to create a class "Vector2"</br>

Task:</br>
Реализовать класс Vector2 со следующиеми методами</br>
• operator <<, >> (перегрузка ввода и вывода)</br>
• Vector2 operator+(const Vector2& other) const;</br>
• Vector2& operator+=(const Vector2& other);</br>
• аналогично - и -=</br>
• float operator*(const Vector2& other) const; (скалярное произведение)</br>
• float operator^(const Vector2& other) const; (векторное произведение)</br>
• * на скаляр с «2х двух сторон»</br>
• / на скаляр</br>
• единичный вектор norm()</br>
• перпендикуляр (y, -x)</br>
• len(), squareLen()</br>
• унарный минус Vector2 operator-() const</br>
• конструкторы</br>
• поворот на угол 2шт.: rotate и getRotated (первый меняет себя, второй –возвращает новое значение … по аналогии с += и +)</br>
