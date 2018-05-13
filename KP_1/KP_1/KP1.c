// Відображення кодів натиснутих клавіш
#include <windows.h>
#define _USE_MATH_DEFINES 
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	//Створення прототипу функції вікна
LPCWSTR szProgName = L"Progname";
//char szProgName[] = "Progname";							//ім'я програми
char szText[] = "";
POINT	tz1[16] = { -50,50,0,50,0,10,20,10,30,8,40,2,47,-9,50,-20,47,-30,
					43,-40,32,-48,20,-50,-25,-50,-25,30,-50,30,-50,50 },
		tz2[8] = { 0,-10,15,-10,22,-13,25,-20,22,-27,15,-30,0,-30,0,-10 };// Вузлові точки літери "Ъ"
POINT	z1[16], z2[8];
double alpha = 0.0, k=1.0;	// alpha - кут нахилу літери,
							// k - масштабний коефіціент
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;			//идентификатор окна
	MSG lpMsg;			//идентификатор сообщения
	WNDCLASS w;			//создаём экземпляр структуры WNDCLASS
						//И начинаем её заполнять
	w.lpszClassName = (LPCWSTR)szProgName;	//имя программы
	w.hInstance = hInstance;		//идентификатор текущего приложения
	w.lpfnWndProc = WndProc;		//указатель на функцию окна
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	w.lpszMenuName = 0;				//меню пока не будет
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//цвет фона окна
	w.style = CS_HREDRAW | CS_VREDRAW;		//стиль - перерисовываемое по х и по у
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	RegisterClass(&w);
	
	//Создадим окно в памяти, заполнив аргументы CreateWindow
	hWnd = CreateWindow(szProgName, //Имя программы
		L"Перегляд кодів клавіатури - 1.0", //Заголовок окна
		WS_OVERLAPPEDWINDOW, //Стиль окна - перекрывающееся
		20,		//положення вікна на екрані по осі х
		20,		//по у
		800,	// ширина (по осі х)
		600,	// висота (по у)
		(HWND)NULL, //ідентифікатор родительского окна
		(HMENU)NULL, //идентификатор меню
		(HINSTANCE)hInstance, //идентификатор экземпляра программы
		(HINSTANCE)NULL); //отсутствие дополнительных параметров
						  //Выводим окно из памяти на экран
	ShowWindow(hWnd, nCmdShow);
	//Обновим содержимое окна
	UpdateWindow(hWnd);
	
	//Цикл обработки сообщений
	while (GetMessage(&lpMsg, NULL, 0, 0)) 
	{										//Получаем сообщение из очереди
		TranslateMessage(&lpMsg);			//Преобразует сообщения клавиш в символы
		DispatchMessage(&lpMsg);			//Передаёт сообщение соответствующей функции окна
	}
	return(lpMsg.wParam);
}
//Функція вікна
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam)
{
	HPEN hpen1;					//об'ява об'єкта перо (олівець)
	HGDIOBJ hbrushOld;
	HBRUSH hbrush;					//об'ява щітки (або валика)
	HDC hdc;						//создаём идентификатор контекста устройства
	PAINTSTRUCT ps;					//створення екземпляра структури графічного виводу
	int x = 800, y = 600, z=0;		// Розміри головного вікна
	unsigned int key = 20;
	//Цикл обробки повідомлень
		for (int i = 0;i < 16;i++)	// Обчислення порточних координат точок контуру літери
		{
			z1[i].x = (long)(250 + tz1[i].x*cos(alpha) + tz1[i].y*sin(alpha));
			z1[i].y = (long)(250 - tz1[i].y*cos(alpha) + tz1[i].x*sin(alpha));
		}
		for (int i = 0;i < 8;i++)
		{
			z2[i].x = (long)(250 + tz2[i].x*cos(alpha) + tz2[i].y*sin(alpha));
			z2[i].y = (long)(250 - tz2[i].y*cos(alpha) + tz2[i].x*sin(alpha));
		}
		switch (messg)
		{
		case WM_PAINT:						// повідомлення малювання	

			hdc = BeginPaint(hWnd, &ps);	// запуск малювання
			SetBkMode(hdc, TRANSPARENT);
			hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//колір лінії - чорний
			hpenOld = (HPEN)SelectObject(hdc, hpen1);
											/* ===================== П О Ч А Т О К  СВОЄЇ ВСТАВКИ =================*/
											//зазвичай саме тут вставляється текст власної розробки:		
			//			MoveToEx(hdc, 150, 50, NULL);	// Встановлення позиції в задану точку
			
			hbrush = CreateSolidBrush(RGB(150, 0, 150));	// Колір (темновишневий) заливка 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
			Polygon(hdc, z1, 16);
			hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Колір білий 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
			Polygon(hdc, z2, 8);
//			
//	switch (messg)
//			{
			case WM_KEYDOWN:
				key = wParam;					//	Отримано код натиснутої клавіші
				_itow_s(key, szText, 4, 10);	//	Перетворення в символьний рядок - параметри:
												// 1-код; 2-результ. перетворення; 
												// 3-число байтів у буфері для символа; 
												// 4-система числення 
				hdc = GetDC(hWnd);				// отримання контексту креслення
				switch (key)					// натиснуто клавішу:
				{	
				case 37:						// проти стрілки годинника (стрілка вліво)
					alpha += M_PI / 4.0;
					TextOut(hdc, 10, 60, szText, 2);
					break;
				case 38:						// Збільшення (вгору)
					k *= 1.2;
					TextOut(hdc, 50, 60, szText, 2);
					break;
				case 39:						// За стрілкою годинника (вправо)
					alpha -= M_PI / 4.0;
					TextOut(hdc, 80, 60, szText, 2);
					break;						
				case 40:						// Зменшення (вниз)
					k *= 0.85;
					TextOut(hdc, 110, 60, szText, 2);
					break;
				case 27:						// Вихід (Esc)
					TextOut(hdc, z, 30, L"Дякую за увагу.   Допобачення!", 31);
					//Sleep(2500);
					TextOut(hdc, z, 60, L"Gjrf", 12);
					Sleep(4500);
					PostQuitMessage(0);			// Повний вихід з програми "КІНЕЦЬ"
					break;
			//	default:
			//		return(DefWindowProc(hWnd, messg, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
				}
				TextOut(hdc, z, 30, L"Тра-ля-ля", 10);
/*
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Колір (білий) заливка 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
				Polygon(hdc, z1, 16);
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Колір білий 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
				Polygon(hdc, z2, 8);
*/
		//}
			
			
		
			/*===    К І Н Е Ц Ь   "СВОЄЇ" ВСТАВКИ    ==============*/
			ValidateRect(hWnd, NULL);		// Обновлення вікна
											//		
			EndPaint(hWnd, &ps);			// Закінчення малювання
			break;
			//Обработка нажатия клавиши
/*		case WM_KEYDOWN:
			key = wParam;					//Отримано код натиснутої клавіші
			_itow_s(key, szText, 4, 10);	//Перетворюємо в символьний рядок:
											// 1-код; 2-результат; 
											//3-байтів у буфері для символа; 
											//4-система числення 
			hdc = GetDC(hWnd);				//Получаем контекст рисования
		//	TextOut(hdc, 10, 20, szText, 2); //Выводим текст на экран
				if (key == 37)
				alpha += M_PI / 4;//Rectangle(hdc, 120, 150, 480, 210);
			else Rectangle(hdc, 120, 250, 480, 290);
		for (int i = 0;i < 16;i++)
			{
				z1[i].x = (long)(250 + tz1[i].x*cos(alpha) + tz1[i].y*sin(alpha));
				z1[i].y = (long)(250 - tz1[i].y*cos(alpha) + tz1[i].x*sin(alpha));
			}
			for (int i = 0;i < 8;i++)
			{
				z2[i].x = (long)(250 + tz2[i].x*cos(alpha) + tz2[i].y*sin(alpha));
				z2[i].y = (long)(250 - tz2[i].y*cos(alpha) + tz2[i].x*sin(alpha));
			}
			hdc = BeginPaint(hWnd, &ps);	// запуск малювання
			hbrush = CreateSolidBrush(RGB(150, 0, 150));	// Колір (темновишневий) заливка 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
			Polygon(hdc, z1, 16);
			hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Колір білий 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
			Polygon(hdc, z2, 8);
			ValidateRect(hWnd, NULL);		// Обновлення вікна
			EndPaint(hWnd, &ps);			// Закінчення малювання
			break;
*/			//сообщение выхода
		case WM_DESTROY:
			PostQuitMessage(0); //Посылаем сообщение выхода с кодом 0 - нормальное завершение
			break;
		default:
			return(DefWindowProc(hWnd, messg, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
		}	
	 return 0;
}
