# 4sem
The program to create a class "Vector2"

Task:
Реализовать класс Vector2 со следующиеми методами:
• operator <<, >> (перегрузка ввода и вывода)
• Vector2 operator+(const Vector2& other) const;
• Vector2& operator+=(const Vector2& other);
• аналогично - и -=
• float operator*(const Vector2& other) const; (скалярное произведение)
• float operator^(const Vector2& other) const; (векторное произведение)
• * на скаляр с «2х двух сторон»
• / на скаляр
• единичный вектор norm()
• перпендикуляр (y, -x)
• len(), squareLen()
• унарный минус Vector2 operator-() const
• конструкторы
• поворот на угол 2шт.: rotate и getRotated (первый меняет себя, второй –
возвращает новое значение … по аналогии с += и +)
