
// AlMmPpg.h : CAlMmPropPage �v���p�e�B �y�[�W �N���X�̐錾���s���܂�

////////////////////////////////////////////////////////////////////////////
// CAlMmPropPage : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlMmPpg.cpp.cpp �t�@�C�����Q�Ƃ��Ă�������

class CAlMmPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAlMmPropPage)
	DECLARE_OLECREATE_EX(CAlMmPropPage)

// �R���X�g���N�^
public:
	CAlMmPropPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAlMmPropPage)
	enum { IDD = IDD_PROPPAGE_ALMM };
	//}}AFX_DATA

// �C���v�������e�[�V����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ���b�Z�[�W �}�b�v
protected:
	//{{AFX_MSG(CAlMmPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


