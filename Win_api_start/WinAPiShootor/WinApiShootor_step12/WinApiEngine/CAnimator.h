#pragma once
#include <windows.h>
#include <unordered_map>
#include "CAniSeq.h"
using namespace std;

class CAniSeq;
class CAPI_Engine;
class CObjectAPI;

class CAnimator
{
public:
	CAnimator();
	CAnimator(const CAnimator& t);
	~CAnimator();

	void operator=(const CAnimator& t);

	void SetId(string tId)
	{
		mId = tId;
	}

	bool Create(CAPI_Engine* tpEngine)
	{
		mpEngine = tpEngine;

		return true;
	}
	void Destroy()
	{

	}

	// ���� �ִϸ��̼� ������ ���� ������ ����
	void UpdateAnimation(float t);
	// ���� �ִϸ��̼��� ��� ��� ������
	void Render(CAPI_Engine* tpEngine, float tX, float tY);

	// �ִϸ��̼� �������� ����ϴ� �Լ�
	bool AddAniSeq(const string& tName, float tTimeInterval, int tTotalFramesCount, LPCWSTR tpFileName, ANI_PO tPlayOption = ANI_PO::LOOP);

	void SetDefaultAniSeq(const string& tStrDefaultAniSeq);
	void PlayAni(const string& tStrAniSeq);

	void LateUpdate();
	
	void SetOwnerObject(CObjectAPI* tpObject)
	{
		mpObject = tpObject;
	}
	CObjectAPI* GetOwnerObject() const
	{
		return mpObject;
	}

public:
	// ���� �ִϸ��̼� �������� ����, Ű�� ���ڿ�Ÿ��, ���� �ִϸ��̼� �������� �ּҰ�
	// ������ ������ �ִϸ��̼� �������� �����ϱ� ���� �ڷᱸ���� �ؽ����̺��� �����ߴ�
	unordered_map<string, CAniSeq*> mAniSeqs;

	string mId = "";					// �� �ִϸ������� �ĺ��� Ű

	CAPI_Engine* mpEngine = nullptr;	// ������ �ּҰ�(����)
	
	CAniSeq* mpCurAniSeq = nullptr;		// ���� �ִϸ��̼� �������� �ּҰ�(����)
	string mStrKeyCurAniSeq;			// ���� �ִϸ��̼� �������� �ĺ��� Ű

	string mStrKeyPrevAniSeq = "";		// ���� �ִϸ��̼� �������� �ĺ��� Ű

	CObjectAPI* mpObject = nullptr;
};
