#include <stdio.h>

int main(void) {
	
	// Ű���� (�����)
	// ���������� ������ ���� �뵵�� ���ǵǾ��ִ� �ܾ�.

	// ���� ����
	// '�ڷ���' '�ĺ����̸�(�����̸�)'
	// ���� �̸����� Ű����� ����� �� ����.
	
	// ������ ����
	int a; // 4byte, ��ȣ�� �ִ� ���� �ڷ���
	//unsigned : �ٸ� �ڷ������� �տ� �پ ��ȣ ���� ���·� ��� �����ϰ� �Ѵ�.
	unsigned int ua; // 4byte, ��ȣ�� ���� ���� �ڷ���
	short s; // 2byte, ��ȣ�� �ִ� ���� �ڷ���
	long l; // 4byte, ��ȣ�� �ִ� ���� �ڷ���
	long long ll ; // 8byte �̻�, ��ȣ�� �ִ� ���� �ڷ���
	char c; // 1byte(��κ�) , ���� �ڷ���
	
	// = : ���Կ����� �����ʿ� �ִ� ���� ���ʿ� �ִ� ������ �����ϴ� ������.
	// ������ ���� �� �־�� �Ҷ��� �������� ,
	// ������ ���� �����ؾ��� ��쿡�� ������ ���
	// 
	// �����Ⱚ : Ȯ���� �޸� ������ �����ִ� �� �� ���� ��
	// �ʱ�ȭ : ����� ������ ���� ó������ ���� �����ϴ� ���� ( �����Ⱚ�� ���ֱ� ���ؼ� )
	a = 4;
	ua = 0; 
	c = 1232; // -128 ~ 127 ���� ����, 1232�� �����ϸ� �����÷ο찡 �߻���.
	s = 1232;
	printf("%d\n", a);
	printf("%d\n", ua);
	printf("%d\n", c);
	printf("%d\n", s);

	// ���� ����� ���ÿ� �ʱ�ȭ
	int a2 = 3;

	// �Ǽ��� ���� ����
	float f; // 4byte , ��ȣ�� �ִ� �Ǽ� �ڷ���
	double d; // 8byte, ��ȣ�� �ִ� �Ǽ� �ڷ���
	long double ld; // 8byte �̻�, �ü���� ���� �ٸ�.

	f = 1.23456789123456789123456789;
	d = 1.23456789123456789123456789;
	ld = 1.23456789123456789123456789;

	printf("%.30f\n", f);
	printf("%.30lf\n", d);
	printf("%.30Lf\n", ld);
	
	//�迭(Array)
	//==============================================

	// 1���� �迭
	int arr1x_1[3] = { 1, 2, 3 };
	int arr1x_2[3] = { 1, 2 };
	double arr1x_3[3] = { 4.4, 5.5, 6.2 };
	double arr1x_4[] = { 3.3, 5.5, 3.3 };
	printf("%d\n", arr1x_1[0]);
	printf("%d\n", arr1x_1[1]);
	printf("%d\n", arr1x_1[2]);

	// 2���� �迭
	short arr2x_1[4][3] = { {1, 2, 3},
							{4, 5, 6},
							{7, 8, 9},
							{0, 1, 2} };
	short arr2x_2[2][3] = { 1, 2, 3, 4, 5, 6 };
	short arr2x_3[][2] = { 1, 2, 3, 4, 5, 6, 7 };
	// 1 3 5 7
	// 2 4 6 0
	printf("%d\n", arr2x_3[3][1]); // �迭�̸�[�� �ε���][�� �ε���]
	//short arr2x_4[2][] = { 1,2,3,4 }; error.
	// �迭�� �ʱ�ȭ �Ҷ� ���� ũ�⸦ �����ָ�
	// �����Ϸ��� �迭�� ũ��� �˾Ƽ� Ȯ���Ѵ�.

	// ���ڿ� ( ������ �迭 )
	// ���ڿ��� �迭 �������� null ���ڰ� �ٴ´�.
	//=======================================================
	char str1[20] = "hello";
	char str2[6] = "hello";
	char str3[5] = "hello"; // null ���ڰ� �� ������ Ȯ������ ����.
	char str4[] = "hella";
	char str5[6] = { 'h', 'e', 'l', 'l', 'o', '\0' };

	printf("%c\n", str5[0]);
	printf("%s\n", str5);

	// �Լ� ȣ�� ����
	// �Լ��̸� (����);
	//strcpy(���ڿ� ����, ���ڿ� ��) : ���ڿ� ������ ���ڿ� ���� ����.
	strcpy(str1, "I don't know you");
	printf("%s\n", str1);
	// strlen(���ڿ� ��) : ���ڿ� ���� ���̸� ��ȯ
	strlen(str1);
	printf("%d\n", strlen(str1));
	printf("%d\n", strlen("I don't know you"));
	// strcmp(���ڿ���1, ���ڿ���2) : ���ڿ��� �ƽ�Ű�ڵ�� ��ȯ������
	// ���ڿ���1�� ��ũ�� ��� ��ȯ, ������ 0 ��ȯ, ������ -1 ��ȯ
	printf("%d\n", strcmp(str3, str4));

	// const Ű����
	// const �� ���ǵ� ������ �ʱ�ȭ ���Ŀ� ���� ������ �� ����
	// ������ ���ó�� ����ϱ� ���� �뵵
	//----------------------------------------------------------
	const int goalPosition = 200;
	
	// LValue �� RValue ?
	// ���Կ����� �������� ���ʿ� �ִ� ���� LValue, �����ʿ� �ִ� ���� RValue ��� ��
	//------------------------------------------------------
	
	// ������ ������ �� ������ ��
	// 1. Ű����(�����) �� �ٸ� �ĺ��� �̸�.
	// 2. �ĺ��� �̸��� �� ������ ���ĺ�, �ƶ��Ƽ���, _(����� : �ַ� �ܾ� ���̿� �����ϱ����� ���)
	

}