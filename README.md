# KG_Kursach
Курсовая работа студента 3-го курса Смирнова С.Ю.

### Задание:
> Элементарные преобразования трехмерных объектов с удалением невидимых линий и поверхностей и построением теней от заданного источника света.

> Вариант 11:
> - Параллелепипед и треугольная пирамида
> - Заливка области с затравкой
> - Алгоритм Zбуфера
> - Построение тени «на землю» (источник света на конечном расстоянии)

## Описание
Проект реализован без использования графических API.  
Интерфейс реализован на Win32Api. Рендер вынесен в отдельный проект.  

![Интерфейс программы](./Resources/examples/main_window_action.gif?raw=true "Интерфейс")

Реализовано управление камерой с помощью перемещения указателя мыши. Нажатие левой клавиши позволяет изменять угол камеры, а средней клавиши (колесико) - положение.

Также доступно изменение параметров фигур и света с помощью колесика мышки при наведении на поле ввода.

## Сборка
- Создать файлы проекта для IDE с помощью premake. (Экземпляр добавлен в папку ``` vendor/premake/ ```)
