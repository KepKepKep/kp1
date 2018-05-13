// ³���������� ���� ���������� �����
#include <windows.h>
#define _USE_MATH_DEFINES 
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	//��������� ��������� ������� ����
LPCWSTR szProgName = L"Progname";
//char szProgName[] = "Progname";							//��'� ��������
char szText[] = "";
POINT	tz1[16] = { -50,50,0,50,0,10,20,10,30,8,40,2,47,-9,50,-20,47,-30,
					43,-40,32,-48,20,-50,-25,-50,-25,30,-50,30,-50,50 },
		tz2[8] = { 0,-10,15,-10,22,-13,25,-20,22,-27,15,-30,0,-30,0,-10 };// ������ ����� ����� "�"
POINT	z1[16], z2[8];
double alpha = 0.0, k=1.0;	// alpha - ��� ������ �����,
							// k - ���������� ����������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;			//������������� ����
	MSG lpMsg;			//������������� ���������
	WNDCLASS w;			//������ ��������� ��������� WNDCLASS
						//� �������� � ���������
	w.lpszClassName = (LPCWSTR)szProgName;	//��� ���������
	w.hInstance = hInstance;		//������������� �������� ����������
	w.lpfnWndProc = WndProc;		//��������� �� ������� ����
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	w.lpszMenuName = 0;				//���� ���� �� �����
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//���� ���� ����
	w.style = CS_HREDRAW | CS_VREDRAW;		//����� - ���������������� �� � � �� �
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	RegisterClass(&w);
	
	//�������� ���� � ������, �������� ��������� CreateWindow
	hWnd = CreateWindow(szProgName, //��� ���������
		L"�������� ���� ��������� - 1.0", //��������� ����
		WS_OVERLAPPEDWINDOW, //����� ���� - ���������������
		20,		//��������� ���� �� ����� �� �� �
		20,		//�� �
		800,	// ������ (�� �� �)
		600,	// ������ (�� �)
		(HWND)NULL, //������������� ������������� ����
		(HMENU)NULL, //������������� ����
		(HINSTANCE)hInstance, //������������� ���������� ���������
		(HINSTANCE)NULL); //���������� �������������� ����������
						  //������� ���� �� ������ �� �����
	ShowWindow(hWnd, nCmdShow);
	//������� ���������� ����
	UpdateWindow(hWnd);
	
	//���� ��������� ���������
	while (GetMessage(&lpMsg, NULL, 0, 0)) 
	{										//�������� ��������� �� �������
		TranslateMessage(&lpMsg);			//����������� ��������� ������ � �������
		DispatchMessage(&lpMsg);			//������� ��������� ��������������� ������� ����
	}
	return(lpMsg.wParam);
}
//������� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam)
{
	HPEN hpen1;					//��'��� ��'���� ���� (������)
	HGDIOBJ hbrushOld;
	HBRUSH hbrush;					//��'��� ����� (��� ������)
	HDC hdc;						//������ ������������� ��������� ����������
	PAINTSTRUCT ps;					//��������� ���������� ��������� ���������� ������
	int x = 800, y = 600, z=0;		// ������ ��������� ����
	unsigned int key = 20;
	//���� ������� ����������
		for (int i = 0;i < 16;i++)	// ���������� ��������� ��������� ����� ������� �����
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
		case WM_PAINT:						// ����������� ���������	

			hdc = BeginPaint(hWnd, &ps);	// ������ ���������
			SetBkMode(hdc, TRANSPARENT);
			hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//���� �� - ������
			hpenOld = (HPEN)SelectObject(hdc, hpen1);
											/* ===================== � � � � � � �  ��Ϊ� ������� =================*/
											//�������� ���� ��� ������������ ����� ������ ��������:		
			//			MoveToEx(hdc, 150, 50, NULL);	// ������������ ������� � ������ �����
			
			hbrush = CreateSolidBrush(RGB(150, 0, 150));	// ���� (�������������) ������� 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
			Polygon(hdc, z1, 16);
			hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ���� ���� 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
			Polygon(hdc, z2, 8);
//			
//	switch (messg)
//			{
			case WM_KEYDOWN:
				key = wParam;					//	�������� ��� ��������� ������
				_itow_s(key, szText, 4, 10);	//	������������ � ���������� ����� - ���������:
												// 1-���; 2-�������. ������������; 
												// 3-����� ����� � ����� ��� �������; 
												// 4-������� �������� 
				hdc = GetDC(hWnd);				// ��������� ��������� ���������
				switch (key)					// ��������� ������:
				{	
				case 37:						// ����� ������ ��������� (������ ����)
					alpha += M_PI / 4.0;
					TextOut(hdc, 10, 60, szText, 2);
					break;
				case 38:						// ��������� (�����)
					k *= 1.2;
					TextOut(hdc, 50, 60, szText, 2);
					break;
				case 39:						// �� ������� ��������� (������)
					alpha -= M_PI / 4.0;
					TextOut(hdc, 80, 60, szText, 2);
					break;						
				case 40:						// ��������� (����)
					k *= 0.85;
					TextOut(hdc, 110, 60, szText, 2);
					break;
				case 27:						// ����� (Esc)
					TextOut(hdc, z, 30, L"����� �� �����.   �����������!", 31);
					//Sleep(2500);
					TextOut(hdc, z, 60, L"Gjrf", 12);
					Sleep(4500);
					PostQuitMessage(0);			// ������ ����� � �������� "ʲ����"
					break;
			//	default:
			//		return(DefWindowProc(hWnd, messg, wParam, lParam)); //����������� ������� ���������� �� �������������
				}
				TextOut(hdc, z, 30, L"���-��-��", 10);
/*
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ���� (����) ������� 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
				Polygon(hdc, z1, 16);
				hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ���� ���� 
				hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
				Polygon(hdc, z2, 8);
*/
		//}
			
			
		
			/*===    � � � � � �   "��Ϊ�" �������    ==============*/
			ValidateRect(hWnd, NULL);		// ���������� ����
											//		
			EndPaint(hWnd, &ps);			// ��������� ���������
			break;
			//��������� ������� �������
/*		case WM_KEYDOWN:
			key = wParam;					//�������� ��� ��������� ������
			_itow_s(key, szText, 4, 10);	//������������ � ���������� �����:
											// 1-���; 2-���������; 
											//3-����� � ����� ��� �������; 
											//4-������� �������� 
			hdc = GetDC(hWnd);				//�������� �������� ���������
		//	TextOut(hdc, 10, 20, szText, 2); //������� ����� �� �����
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
			hdc = BeginPaint(hWnd, &ps);	// ������ ���������
			hbrush = CreateSolidBrush(RGB(150, 0, 150));	// ���� (�������������) ������� 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
			Polygon(hdc, z1, 16);
			hbrush = CreateSolidBrush(RGB(255, 255, 255));	// ���� ���� 
			hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);	// ��������� "������"
			Polygon(hdc, z2, 8);
			ValidateRect(hWnd, NULL);		// ���������� ����
			EndPaint(hWnd, &ps);			// ��������� ���������
			break;
*/			//��������� ������
		case WM_DESTROY:
			PostQuitMessage(0); //�������� ��������� ������ � ����� 0 - ���������� ����������
			break;
		default:
			return(DefWindowProc(hWnd, messg, wParam, lParam)); //����������� ������� ���������� �� �������������
		}	
	 return 0;
}
