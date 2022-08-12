#include "CAnimator.h"
//#include "CAniSeq.h"
#include "APIMacro.h"
#include "CAPI_Engine.h"
#include "CTexture.h"

CAnimator::CAnimator(): mId(""), mpEngine(nullptr), mpCurAniSeq(nullptr), mStrKeyCurAniSeq("")
{
	mAniSeqs.clear();
}
CAnimator::CAnimator(const CAnimator& t)
{
	//�⺻����, shallow copy
	//*this = t;

	mId = t.mId;
	mpEngine = t.mpEngine;
	mpCurAniSeq = t.mpCurAniSeq;
	mStrKeyCurAniSeq = t.mStrKeyCurAniSeq;
	mStrKeyPrevAniSeq = t.mStrKeyPrevAniSeq;
	mpObject = t.mpObject;

	// �ִϸ��̼� �������� ���ӿ�����Ʈ���� �־�� �ϹǷ� ������ ����
	// depp copy
	mAniSeqs.clear();
	unordered_map<string, CAniSeq*>::const_iterator tItor;
	for (tItor = t.mAniSeqs.begin(); tItor != t.mAniSeqs.end(); ++tItor)
	{
		CAniSeq* tpAniClip = new CAniSeq();

		*tpAniClip = *tItor->second;

		mAniSeqs.insert(make_pair(tpAniClip->mId, tpAniClip));
	}
}
CAnimator::~CAnimator()
{
	unordered_map<string, CAniSeq*>::iterator tItor;
	for (tItor = mAniSeqs.begin(); tItor != mAniSeqs.end(); ++tItor)
	{
		SAFE_DELETE(tItor->second);
	}

	mAniSeqs.clear();
}

void CAnimator::operator=(const CAnimator& t)
{
	/*if (&t == this)
	{
		return;
	}*/

	//�⺻����, shallow copy
	//*this = t;

	mId = t.mId;
	mpEngine = t.mpEngine;
	mpCurAniSeq = t.mpCurAniSeq;
	mStrKeyCurAniSeq = t.mStrKeyCurAniSeq;
	mStrKeyPrevAniSeq = t.mStrKeyPrevAniSeq;
	mpObject = t.mpObject;


	// �ִϸ��̼� �������� ���ӿ�����Ʈ���� �־�� �ϹǷ� ������ ����
	// depp copy
	mAniSeqs.clear();
	unordered_map<string, CAniSeq*>::const_iterator tItor;
	for (tItor = t.mAniSeqs.begin(); tItor != t.mAniSeqs.end(); ++tItor)
	{
		CAniSeq* tpAniClip = new CAniSeq();

		*tpAniClip = *tItor->second;

		mAniSeqs.insert(make_pair(tpAniClip->mId, tpAniClip));
	}
}

void CAnimator::UpdateAnimation(float t)
{
	/*
	// test
	this->mStrKeyCurAniSeq = "ani_idle_actor";
	*/

	unordered_map<string, CAniSeq*>::iterator tItor = mAniSeqs.find(mStrKeyCurAniSeq);
	mpCurAniSeq = tItor->second;

	mpCurAniSeq->Update(t);
}

void CAnimator::Render(CAPI_Engine* tpEngine, float tX, float tY)
{
	int tIndex = mpCurAniSeq->mCurFrameIndex;
	CTexture* tpTex = mpCurAniSeq->mTexs[tIndex];
	if (tpTex)
	{
		tpEngine->DrawTexture(tX, tY, tpTex);
	}
	
	LateUpdate();

	/*
	// late update
	switch (mpCurAniSeq->mPlayOption)
	{
	case ANI_PO::LOOP:
	{
	}
	break;
	case ANI_PO::ONCE:
	{
		// ������ �����ӱ��� �ִϸ��̼� �Ǿ��ٸ� / �ִϸ��̼� �ð��� �� �Ǿ��ٸ�
		// ������ �ϴ� �ִϸ��̼��� �÷����Ѵ�
		if (mpCurAniSeq->mCurFrameIndex == mpCurAniSeq->mTotalFramesCount - 1)
		{
			mStrKeyCurAniSeq = mStrKeyPrevAniSeq;

			mpCurAniSeq->mCurFrameIndex = 0;
			mpCurAniSeq->mAniTime = 0.0f;
		}
	}
	break;
	}*/
}

bool CAnimator::AddAniSeq(const string& tName, float tTimeInterval, int tTotalFramesCount, LPCWSTR tpFileName, ANI_PO tPlayOption)
{
	// �����Ҵ�
	CAniSeq* tpClip = new CAniSeq();

	// ������ ����
	tpClip->mId = tName;

	tpClip->mTimeInterval = tTimeInterval;
	tpClip->mTotalFramesCount = tTotalFramesCount;
	tpClip->mPlayOption = tPlayOption;

	// ������ ������ŭ ���� �߰��Ѵ�
	// �̸��� �ڿ� ��ȣ�� �ٿ� ����� ������ ��Ģ�� ���Ͽ���
	for (int ti = 0; ti < tTotalFramesCount; ++ti)
	{
		CTexture* tpTex = nullptr;
		tpTex = new CTexture();

		WCHAR szTemp[256] = { 0 };
		wsprintf(szTemp, L"%s_%d.bmp", tpFileName, ti);

		tpTex->LoadTexture(mpEngine->GetHInst(), mpEngine->GetHDC(), szTemp);
		tpClip->mTexs.push_back(tpTex);
	}

	// �ؽ����̺��� ���
	mAniSeqs.insert(make_pair(tName, tpClip));

	return true;
}

void CAnimator::SetDefaultAniSeq(const string& tStrDefaultAniSeq)
{
	mStrKeyPrevAniSeq = tStrDefaultAniSeq;
	mStrKeyCurAniSeq = tStrDefaultAniSeq;

	if (nullptr != mpCurAniSeq)
	{
		mpCurAniSeq->mCurFrameIndex = 0;
		mpCurAniSeq->mAniTime = 0.0f;
	}
}

void CAnimator::PlayAni(const string& tStrAniSeq)
{
	// ���� �÷��� ���� �ִϸ��̼� ������ Ű�� �����ص�
	mStrKeyPrevAniSeq = mStrKeyCurAniSeq;

	// ���� �÷����� �ִϸ��̼� ������ ������ ����
	mStrKeyCurAniSeq = tStrAniSeq;

	if (nullptr != mpCurAniSeq)
	{
		mpCurAniSeq->mCurFrameIndex = 0;
		mpCurAniSeq->mAniTime = 0.0f;
	}
}

void CAnimator::LateUpdate()
{
	switch (mpCurAniSeq->mPlayOption)
	{
	case ANI_PO::LOOP:
	{
	}
	break;
	case ANI_PO::ONCE:
	{
		// ������ �����ӱ��� �ִϸ��̼� �Ǿ��ٸ� / �ִϸ��̼� �ð��� �� �Ǿ��ٸ�
		// ������ �ϴ� �ִϸ��̼��� �÷����Ѵ�
		if (mpCurAniSeq->mCurFrameIndex == mpCurAniSeq->mTotalFramesCount - 1)
		{
			if (mpCurAniSeq->mAniTime >= mpCurAniSeq->mTimeInterval)
			{
				//mStrKeyCurAniSeq = mStrKeyPrevAniSeq;

				mpCurAniSeq->mCurFrameIndex = 0;
				mpCurAniSeq->mAniTime = 0.0f;
			}
		}
	}
	break;

	}
}