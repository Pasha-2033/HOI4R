# 3D движок HOI4R
Вдохновленный игрой HOI4, я решил создать свой движок. Я хотел добиться сохранение контента Paradox и при этом улучшить игровой контент.
На данный момент прогресс совсем небольшой:
<dl>
    <dd>создано окно hwnd, упрощена работа с ним (модуль winmodule)</dd>
    <dd>создано устройство directx, поддерживает на данный момент 9, 10 и 11 (модуль dxmodule)</dd>
    <dd>создано окно dxwindowclass, нацеленное на совмещение работы directx и  hwnd (модуль dxwindow)</dd>
</dl>
# Планы
<dl>
    <dd>создать класс графического объекта</dd>
    <dd>доделать dxwindowclass</dd>
    <dd>создать систему динамических токенов</dd>
</dl>
# О системе динамических токенов
Обычно игра это exe которая импортирует dll движка, тем самым образом игра задает правила обработки входных данных, а движок их добывает, обрабатывает, работает с графикой.
Я подумал, почему бы не дать игрокам модифицировать не только контент, но и саму игру.
Это означает, что игра не должна быть написана на С++, потому что это приведет к куче exe.
Поэтому я подумал, почему бы не создавать игровую логику также через текстовые файлы со своим "псевдоЯП".
То есть сделать так, чтобы движок был готов создать шаблоны токенов из функций и полей, который движок готов предоставить.
<nobr>
Например (примернное представление кода):
token = {
	name = variable			#указываем, как называется конструктор создания токена
	type = object			#указываем, что токен это обьект, а значит предназначен в основном для хранения данных (можно также прописать и функции, но тогда работа с объектом с большим кол-во полей будет неудобно)
	creation_scope = any	#указываем, что область создания любая
	access_scope = any		#указываем, что область получения экземпляра токена любая
	add_creation_input = {	#указываем, что при создании экземпляра обязательно нужно указать параметр
		name = name_input	#имя параметра при создании "name_input"
		type = string		#указываем его тип
	}
	instance_name = creation_input:name_input	#для обращения к экземпляру используем значение creation_input:name_input
	add_field = {				#добавляем в конструктор поле
		name = value			#сообщаем имя поля (для обращения)
		type = UINT				#сообщаем тип поля
		value = 0				#указываем значение по умолчанию
		getfunction = {			#указываем как получить это значение (все поля private, так что эти функции буду защищать от неверных действий со значениями)
			name = getvalue		#имя функции
		}
		setfunction = {			#указываем как установить это значение (все поля private, так что эти функции буду защищать от неверных действий со значениями)
			name = setvalue
		}
	}
	add_field = {
		name = name
		type = string
		value = creation_input:name_input	#добавляем в экземпляр поле со значеним его имени
		getfunction = {
			name = getname
		}
		setfunction = {
			name = setname
		}
	}
	add_function = {			#добавляем функцию к конструктору
		name = add				#имя функции "add"
		add_argument = {		#функци принимает аргумент
			name = addvalue		#имя аргумента "addvalue"
			type = UINT			#указываем тип аргументв
		}
		add_action = {				#чтобы функция дала результат, мы должны вызывать функции движка, чтобы тот произвел операции
			type = add				#тип функции add, простое сложение
			target = value			#указываем, куда результат будет складирован
			add_source = addvalue	#указываем какой аргумент получит функция (важно указывать в порядке который соответствует функции внути движка)
			add_source = value		#указываем какой аргумент получит функция (важно указывать в порядке который соответствует функции внути движка)
		}
	}
}
Таким образом создание переменной будет таким:
new variable(name_input = "myvar1")
new variable(name_input = "myvar2")
А их сложение таким:
myvar1:setvalue = 5
myvar2:setvalue = 7
myvar1:add = {
	addvalue = myvar2:getvalue
}
тогда myvar1:getvalue даст 12
</nobr>