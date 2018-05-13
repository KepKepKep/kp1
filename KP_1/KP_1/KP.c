// Відображення кодів натиснутих клавіш "Ъ' і "Б"
// Маніпуляції: збільшення\зменшення\повороти

#include <windows.h>		// Підключення стандартних бібліотек
#define _USE_MATH_DEFINES	// Підключення математичних констант
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Прототип функції вікна(Опис внизу)
LPCWSTR szProgName = L"Progname";						// ім'я програми
char szText[] = "";										// Рядок для поточного використання
			/*	Обява та ініціалізація масивів точок контурів літер завдання. 
				В даномк прикладі - це літери українська Б та російський твердий знак Ъ */
POINT	tz1[16] = { -50,50,0,50,0,10,20,10,30,8,40,2,47,-9,50,-20, 47,-30, // Зовнішній контур Ъ
					43,-40,32,-48,20,-50,-25,-50,-25,30,-50,30,-50,50 },	
	tz2[8] = { 0,-10,15,-10,22,-13,25,-20, 22,-27,15,-30,0,-30,0,-10 },		// Внутрішній контур Ъ
	bz1[16] = { -35,50,35,50,35,30,-10,30,-10,10,10,10,20,8,30,2,38,-9,40,	// Зовнішній контур Б
				-20,38,-30,33,-40,23,-48,10,-50,-35,-50,-35,50 },
	bz2[8] = {-10,-10,3,-10,13,-13,15,-20,13,-27,3,-30,-10,-30,-10,-10};	// Внутрішній контур Б
POINT	z1[16], z2[8];
double alpha = 0.0, k=1.0;	// alpha - кут нахилу літери, Глобальні змінні
							// k - масштабний коефіціент
				/* Далі - Головна програма (функція Main для Win32) */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;			// ідентифікатор вікна
	MSG lpMsg;			// ідентифікатор повідомлення
	WNDCLASS w;			// створення екземпляра структури WNDCLASS
						// і далі - заповнення щойно створеної структури
	w.lpszClassName = (LPCWSTR)szProgName;	// ім'я програми
	w.hInstance = hInstance;		// ідентифікатор поточного додатку
	w.lpfnWndProc = WndProc;		// вказівник на функцію вікна
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	w.lpszMenuName = 0;				//меню покищо не будет
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// колір фону вікна
	w.style = CS_HREDRAW | CS_VREDRAW;		//стиль - перемальовування по х і по у
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	RegisterClass(&w);						// Реєстрація екземпляра нашого вікна 
											// яке належить до класу WNDCLASS
		/* Створення цього вікна в пам'яті, заповнюючи аргументи функції CreateWindow */
	hWnd = CreateWindow(szProgName,			// Ім'я програми
		L"Перегляд кодів клавіатури - 1.0", // Заголовок вікна
		WS_OVERLAPPEDWINDOW,				// Стиль вікна - перетинається
		20,									// положення вікна на екрані по осі х
		20,									// по у
		800,								// ширина (по осі х)
		600,								// висота (по у)
		(HWND)NULL,							// ідентифікатор батьківського вікна
		(HMENU)NULL,						// ідентифікатор меню
		(HINSTANCE)hInstance,				// ідентифікатор екземпляра програми
		(HINSTANCE)NULL);					// без додаткових параметрів
						  //Виведення вікно з пам'яті на екран
	ShowWindow(hWnd, nCmdShow);
	
	UpdateWindow(hWnd);		//Оновлення змісту вікна

							//Цикл обробки повідомлень
	while (GetMessage(&lpMsg, NULL, 0, 0))
	{										// Отримання повідомлення з черги
		TranslateMessage(&lpMsg);			// Перетворення повідомлення клавіш в символи
		DispatchMessage(&lpMsg);			// Передавання повідомлення відповідній функції вікна
	}
	return(lpMsg.wParam);
}		/* Закінчилася Головна функція*/

/*=======  Функція вікна, прототип якої записаний в рядку 8  == */
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam)
{
	HPEN hpen1;						//об'ява об'єкта перо (олівець)
	HGDIOBJ hbrushOld, hpenOld;
	HBRUSH hbrush;					//об'ява щітки (або валика)
	HDC hdc;						//створення ідентифікатора контексту пристрою
	PAINTSTRUCT ps;					//створення екземпляра структури графічного виводу
	int x = 800, y = 600, z=0,		// Розміри головного вікна
	bx, by;							// буферні змінні
	unsigned int key = 20;			// Змінна отримання коду клавіші
			/*	Цикл обробки повідомлень	*/
	for (int i = 0;i < 16;i++)	// Обчислення початкових координат точок контуру літери
	{
		z1[i].x = (long)(250 + tz1[i].x*cos(alpha) + tz1[i].y*sin(alpha));	//Зовнішнього
		z1[i].y = (long)(250 - tz1[i].y*cos(alpha) + tz1[i].x*sin(alpha));
	}
	for (int i = 0;i < 8;i++)
	{
		z2[i].x = (long)(250 + tz2[i].x*cos(alpha) + tz2[i].y*sin(alpha));	//Внутрішнього
		z2[i].y = (long)(250 - tz2[i].y*cos(alpha) + tz2[i].x*sin(alpha));
	}
	switch (messg)		/*	Обробка запитів до ОС, коди яких отримує змінна messg	*/
	{
		case WM_PAINT:						// повідомлення малювання		
			hdc = BeginPaint(hWnd, &ps);	// запуск малювання
			SetBkMode(hdc, TRANSPARENT);	// Режим перемальовування
			hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//колір лінії - чорний
			hpenOld = (HPEN)SelectObject(hdc, hpen1);
			/* ===================== П О Ч А Т О К  СВОЄЇ ВСТАВКИ =================*/
			/* зазвичай саме тут вставляється текст власної розробки:*/							
			hbrush = CreateSolidBrush(RGB(150, 0, 150));	// Колір (темновишневий) заливка 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
			Polygon(hdc, z1, 16);							// Зафарбований багатокутник
			hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Колір заливки білий 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
			Polygon(hdc, z2, 8);							// "Вибілення" внурішн. контуру
			hpen1 = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));	// Товщина лінії 3 пікс.
			hpenOld = (HPEN)SelectObject(hdc, hpen1);		// Запуск пера для креслення лінії
			/*  Побудова рядків підказок у вікні праворуч:	*/
			TextOut(hdc, 600, 30, L"Вибір літери   : Ъ або Б" , 25);
			TextOut(hdc, 600, 60, L"Збільшення    :", 15);	
			MoveToEx(hdc, 720, 75, NULL);
			LineTo(hdc, 720, 55);							// Формування 
			LineTo(hdc, 715, 63);							// стрілочки
			LineTo(hdc, 725, 63);							// вгору
			LineTo(hdc, 720, 55);
			TextOut(hdc, 600, 90, L"Зменшення     :", 15);
			MoveToEx(hdc, 720,105, NULL);
			LineTo(hdc, 715, 98);							// Формування 
			LineTo(hdc, 725, 98);							// стрілочки
			LineTo(hdc, 720, 105);							// вниз
			LineTo(hdc, 720, 85);
			TextOut(hdc, 590 , 120, L"Поворот вліво    :", 18);
			MoveToEx(hdc, 715, 130, NULL);
			LineTo(hdc, 721, 125);							// Формування 
			LineTo(hdc, 721, 135);							// стрілочки
			LineTo(hdc, 715, 130);
			LineTo(hdc, 735, 130);
			TextOut(hdc, 590, 150, L"Поворот вправо :", 16);
			MoveToEx(hdc, 715, 160, NULL);
			LineTo(hdc, 735, 160);							// Формування 
			LineTo(hdc, 729, 155);							// стрілочки
			LineTo(hdc, 729, 165);							// вправо
			LineTo(hdc, 735, 160);
			TextOut(hdc, 600, 190, L"В И Х І Д           :  'Esc'", 29);
			
		case WM_KEYDOWN:
			key = wParam;					//	Отримано код натиснутої клавіші
			_itow_s(key, szText, 4, 10);	//	Перетворення в символьний рядок - параметри:
											// 1-код; 2-результ. перетворення; 
											// 3-число байтів у буфері для символа; 
											// 4-система числення 
			hdc = GetDC(hWnd);				// отримання контексту креслення
			switch (key)					// натиснуто клавішу:
			{	
				case 188:						// Вибір літери "Б"
					for (int i = 0;i < 16;i++)	// Формування масиву точок літери Б
					{
						bx = tz1[i].x; tz1[i].x = bz1[i].x; bz1[i].x = bx;
						by = tz1[i].y; tz1[i].y = bz1[i].y; bz1[i].y = by;
					};
					for (int i = 0;i < 8;i++)
					{
						bx = tz2[i].x; tz2[i].x = bz2[i].x; bz2[i].x = bx;
						by = tz2[i].y; tz2[i].y = bz2[i].y; bz2[i].y = by;
					};
					break;
				case 221:						// Літера"Ъ"
					for (int i = 0;i < 16;i++)	// Формування масиву точок літери Ъ
					{
						bx = tz1[i].x; tz1[i].x = bz1[i].x; bz1[i].x = bx;
						by = tz1[i].y; tz1[i].y = bz1[i].y; bz1[i].y = by;
					};
					for (int i = 0;i < 8;i++)
					{
						int bx = tz2[i].x; tz2[i].x = bz2[i].x; bz2[i].x = bx;
						int by = tz2[i].y; tz2[i].y = bz2[i].y; bz2[i].y = by;
					};
					break;
				case 39:						// проти стрілки годинника (стрілка вліво)
					alpha =alpha+ M_PI / 6.0;	// Кут нахилу збільшено на 30 градусів
					break;
				case 38:						// Збільшення (стрілка вгору)
					k *= 1.2;					// на 20%
					break;
				case 37:						// За стрілкою годинника (стрілка вправо)
					alpha = alpha- M_PI / 6.0;	// Кут нахилу зменшено на 30 градусів
					break;						
				case 40:						// Зменшення (стрілка вниз)
					k *= 0.85;					// на 15%
					break;
				case 27:						// Вихід (Esc)
					TextOut(hdc, 150, 630, L"Дякую за увагу.   Допобачення!", 31);
					Sleep(1500);
					PostQuitMessage(0);			// Повний вихід з програми "КІНЕЦЬ"
					break;
			}
				/*  "Гашення" літери - створення підходящих кольорів для фарбування  */
				hpen1 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));	//колір лінії - білий
				hpenOld = (HPEN)SelectObject(hdc, hpen1);
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Заливка фігури (білим)
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
				Rectangle(hdc, 0, 0, 590, 600);					// Очищення поля примусове
				for (int i = 0;i < 16;i++)	// Обчислення ноових координат точок контуру літери
				{
					z1[i].x = (long)(250 + (tz1[i].x*cos(alpha) + tz1[i].y*sin(alpha))*k);
					z1[i].y = (long)(250 - (tz1[i].y*cos(alpha) - tz1[i].x*sin(alpha))*k);
				}
				for (int i = 0;i < 8;i++)
				{
					z2[i].x = (long)(250 + (tz2[i].x*cos(alpha) + tz2[i].y*sin(alpha))*k);
					z2[i].y = (long)(250 - (tz2[i].y*cos(alpha) - tz2[i].x*sin(alpha))*k);
				}
				hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//колір лінії - чорний
				hpenOld = (HPEN)SelectObject(hdc, hpen1);		//відновлення кольорів для малювання

				hbrush = CreateSolidBrush(RGB(150, 0, 150));	// Колір (темновишневий) заливка 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
				Polygon(hdc, z1, 16);
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// Колір білий 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// Включення "маляра"
				Polygon(hdc, z2, 8);			
		
			/*===    К І Н Е Ц Ь   "СВОЄЇ" ВСТАВКИ    ==============*/
			ValidateRect(hWnd, NULL);		// Обновлення вікна	
			EndPaint(hWnd, &ps);			// Закінчення малювання
			break;
	
		case WM_DESTROY:
			PostQuitMessage(0); //Посилаемо повідомлення выходу з кодом 0 - нормальне завершення
			break;
		default:				//Звільнення черги додатку від нерозпізнаних запитів
			return(DefWindowProc(hWnd, messg, wParam, lParam)); 
			
		}	
	 return 0;
}
