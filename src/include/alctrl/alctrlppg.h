
// AlCtrlPpg.h : CAlCtrlPropPage �v���p�e�B �y�[�W �N���X�̐錾���s���܂�

////////////////////////////////////////////////////////////////////////////
// CAlCtrlPropPage : ���̃N���X�̓���̒�`�Ɋւ��Ă� AlCtrlPpg.cpp.cpp �t�@�C�����Q�Ƃ��Ă�������

class CAlCtrlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAlCtrlPropPage)
	DECLARE_OLECREATE_EX(CAlCtrlPropPage)

// �R���X�g���N�^
public:
	CAlCtrlPropPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAlCtrlPropPage)
	enum { IDD = IDD_PROPPAGE_ALCTRL };
	BOOL	m_transparent;
	int 	m_borderWidth;
	//}}AFX_DATA

// �C���v�������e�[�V����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ���b�Z�[�W �}�b�v
protected:
	//{{AFX_MSG(CAlCtrlPropPage)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂�
		//	  ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������I
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


