#include "stdafx.h"
#include "RsaKit.h"
#include "HelpDlg.h"

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDlg)
	//}}AFX_DATA_INIT
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDlg)
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CHelpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    TV_INSERTSTRUCT TreeCtrlItem;

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.pszText = _T("");
	TreeCtrlItem.item.lParam = 1;
	HTREEITEM hTreeItem0 = m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.pszText = _T("ԭ�����");
	TreeCtrlItem.item.lParam = 0;
	HTREEITEM hTreeItem1 = m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("�����洢");
	TreeCtrlItem.item.lParam = 2;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("�ӷ�");
	TreeCtrlItem.item.lParam = 3;
	m_Tree.InsertItem(&TreeCtrlItem);  
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("����");
	TreeCtrlItem.item.lParam = 4;
	m_Tree.InsertItem(&TreeCtrlItem); 

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("�˷�");
	TreeCtrlItem.item.lParam = 5;
	m_Tree.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("����");
	TreeCtrlItem.item.lParam = 6;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("ȡģ");
	TreeCtrlItem.item.lParam = 7;
	m_Tree.InsertItem(&TreeCtrlItem); 
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("��Ԫһ�η���");
	TreeCtrlItem.item.lParam = 8;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("��ģ����");
	TreeCtrlItem.item.lParam = 9;
	m_Tree.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("��������");
	TreeCtrlItem.item.lParam = 10;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("�������");
	TreeCtrlItem.item.lParam = 11;
	m_Tree.InsertItem(&TreeCtrlItem);

	return TRUE;
}

void CHelpDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	switch(pNMTreeView->itemNew.lParam)
	{
	case 0:
		break;
	
	case 2:
		m_Text="";
		m_Text+="\r\n";
		m_Text+="    RSA �����������㣬Ŀǰ����RSA �㷨��������512 ��1024λ�Ĵ�������֮�ϡ�";
		m_Text+="\r\n";
		m_Text+="��������ı�����ֻ��֧�ֵ�64λ���������㣬����������������ʹ�õ���������С";
		m_Text+="\r\n";
		m_Text+="�ڵ���64λ������0xffffffffffffffff��Ҳ����18446744073709551615����ԶԶ�ﲻ";
		m_Text+="\r\n";
		m_Text+="��RSA ����Ҫ��������Ҫר�Ž�������������������һ���⡣";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ��򵥵İ취�ǽ���������������д���Ҳ���ǽ�������0��9��ʮ���������";
		m_Text+="\r\n";
		m_Text+="��������б�ʾ��Ȼ��ģ�������ֹ����С���ʽ���㡱�Ĺ��̱�д��Ӽ��˳�������";
		m_Text+="\r\n";
		m_Text+="����������Ч�ʺܵͣ���Ϊ������Ϊ1024λ�Ĵ�����ʮ����Ҳ�����ٶ�λ�������κ�";
		m_Text+="\r\n";
		m_Text+="һ�����㣬����Ҫ�����������ٸ�Ԫ�ص�����ռ���������ѭ��������Ҫ�������";
		m_Text+="\r\n";
		m_Text+="�ռ��ż���Ľ���λ��־���м��������⣬����ĳЩ�����������ԣ����ö���";
		m_Text+="\r\n";
		m_Text+="�ƻ�ʹ������̴��򻯣����ִ�����ʾ����ת���ɶ�������Ȼ�ǳ��鷳��������ĳ";
		m_Text+="\r\n";
		m_Text+="Щʵ������ɴ�����˶���������ķ�������¼����������Ч�ʾ͸����ˡ�";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    һ����Ч�ĸĽ������ǽ�������ʾΪһ��n �������飬����Ŀǰ��32λϵͳ����";
		m_Text+="\r\n";
		m_Text+="n ����ȡֵΪ2 ��32�η�����0x10000000�����罫һ��������Ϊ1024λ�Ĵ���ת����";
		m_Text+="\r\n";
		m_Text+="0x10000000���ƣ����ͱ����32λ����ÿһλ��ȡֵ��Χ�Ͳ��Ƕ����Ƶ�0��1��ʮ��";
		m_Text+="\r\n";
		m_Text+="�Ƶ�0��9������0-0xffffffff���������ÿ�����һ���޷��ų���������ʾ��һ��ֵ��";
		m_Text+="\r\n";
		m_Text+="����1024λ�Ĵ�������һ����32��Ԫ�ص�unsigned long���飬���unsigned long��";
		m_Text+="\r\n";
		m_Text+="����и������������ѭ����ģ����32�ζ��ѡ�����0x100000000 ����������ƣ���";
		m_Text+="\r\n";
		m_Text+="�ڼ������˵��������һ���£�ת���ǳ����ס�";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    �������18446744073709551615������ ffffffff ffffffff�����൱��ʮ���Ƶ�";
		m_Text+="\r\n";
		m_Text+="99������λ��ÿλ����ffffffff����18446744073709551616 ����00000001 00000000";
		m_Text+="\r\n";
		m_Text+="00000000�����൱��ʮ���Ƶ�100������λ����һλ��1 ��������λ��0����˵ȵȡ�";
		m_Text+="\r\n";
		m_Text+="��ʵ��Ӧ���У������֡����������˳����õ�λ��ǰ��λ�ں�ķ�ʽ������������";
		m_Text+="\r\n";
		m_Text+="A �Ϳ��Է��������ѧ���ʽ����ʾ��ֵ��A=Sum[i=0 to n](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="������Sum ��ʾ��ͣ�A[i]��ʾ���Լ�¼A ������ĵ�i ��Ԫ�أ�**��ʾ�˷�����";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    �κ������������ն��ֽܷ������������֮������㣬��0x100000000 ��������";
		m_Text+="\r\n";
		m_Text+="�����֡����ﵽ0xffffffff��������������֮������㣬���Ҳ��Ȼ������Ŀǰ32";
		m_Text+="\r\n";
		m_Text+="ϵͳ���ֳ�����VC++�У�����һ��__int64 ���Ϳ��Դ���64λ�����������Բ��õ���";
		m_Text+="\r\n";
		m_Text+="��һ���⣬������������ϵͳ�����������64λ���Σ�����Ҫ���ø�С�Ľ��Ʒ�ʽ��";
		m_Text+="\r\n";
		m_Text+="�洢����������WORD���ͣ�16λ������������ʾ0x10000 ���ƣ���Ч�ʸ��ߵİ취��";
		m_Text+="\r\n";
		m_Text+="�ǲ���32λ��DWORD ���ͣ�ֻ������0x100000000 ���Ƹĳ�0x40000000���ƣ�������";
		m_Text+="\r\n";
		m_Text+="�����ֽ�����������������Ϊ 0x3fffffff * 0x3fffffff��С��0xffffffff��ֻ";
		m_Text+="\r\n";
		m_Text+="�ǲ��ܼ򵥵��ø�λ��λ������������ֳ����������֡���";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 3:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="�裺";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)��p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A+B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="��Ȼ��";
		m_Text+="\r\n";
		m_Text+="  C[i]���Ǽ򵥵ص���A[i]+B[i]����Ϊ���C[i]>0xffffffff����Ҫ��λ����Ȼ����";
		m_Text+="\r\n";
		m_Text+="  C[i-1]ʱҲ���ܲ����˽�λ�����Լ���C[i]ʱ��Ҫ�����ϴεĽ�λֵ��";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="�����carry[i]��¼ÿ�εĽ�λ���У�";
		m_Text+="\r\n";
		m_Text+="  C[i]=A[i]+B[i]+carry[i-1]-carry[i]*0x100000000";
		m_Text+="\r\n";
		m_Text+="  ����carry[-1]=0";
		m_Text+="\r\n";
		m_Text+="  ��A[i]+B[i]+carry[i-1]>0xffffffff����carry[i]=1����֮��carry[i]=0";
		m_Text+="\r\n";
		m_Text+="  ��carry[p]=0����n=p����֮��n=p+1";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 4:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="�裺";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)��p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A-B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="��Ȼ��";
		m_Text+="\r\n";
		m_Text+="  C[i]���Ǽ򵥵ص���A[i]-B[i]����Ϊ���A[i]<B[i]����Ҫ��λ����Ȼ����";
		m_Text+="\r\n";
		m_Text+="  C[i-1]ʱҲ���ܲ����˽�λ�����Լ���C[i]ʱ��Ҫ��ȥ�ϴεĽ�λֵ��";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="�����carry[i]��¼ÿ�εĽ�λ���У�";
		m_Text+="\r\n";
		m_Text+="  C[i]=A[i]+carry[i]*0x100000000-B[i]-carry[i-1]";
		m_Text+="\r\n";
		m_Text+="  ����carry[-1]=0";
		m_Text+="\r\n";
		m_Text+="  ��A[i]>B[i]��carry[i]=0����֮��carry[i]=1";
		m_Text+="\r\n";
		m_Text+="  ��C[p]=0����n=p-1����֮��n=p";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 5:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="�裺";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)��p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A*B";
		m_Text+="\r\n";
		m_Text+="";
		m_Text+="\r\n";
		m_Text+="��Ȼ��";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to q](A*B[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  ��(A*B[i]*100000000**i)=Sum[j=0 to p](A[j]*B[i]*0x100000000**(i+j))";
		m_Text+="\r\n";
		m_Text+="  ����C=Sum[i=0 to q](Sum[j=0 to p](A[j]*B[i]*0x100000000**(i+j)))";
		m_Text+="\r\n";
		m_Text+="";
		m_Text+="\r\n";
		m_Text+="��ˣ�";
		m_Text+="\r\n";
		m_Text+="  C[i]=Sum[j=0 to q](A[i-j]*B[j])+carry[i-1]-carry[i]*0x100000000";
		m_Text+="\r\n";
		m_Text+="  ����carry[-1]=0";
		m_Text+="\r\n";
		m_Text+="  carry[i]=(Sum[j=0 to q](A[i-j]*B[j])+carry[i-1])/0x100000000";
		m_Text+="\r\n";
		m_Text+="  n=p+q-1����carry[n]>0����n=n+1��C[n]=carry";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 6:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="�裺";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)��p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A/B";
		m_Text+="\r\n";
		m_Text+="";
		m_Text+="\r\n";
		m_Text+="�����޷���B ��A �����̡�������ֻ��ת����B[q]��A[p]���������õ�һ������ֵ��";
		m_Text+="\r\n";
		m_Text+="�������ǲ��ܹ�ֱ�Ӽ���C�����ǣ����ǿ���һ��һ���رƽ�C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="��Ȼ��";
		m_Text+="\r\n";
		m_Text+="  (A[p]/B[q]-1)*0x100000000**(p-q)<C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="�";
		m_Text+="\r\n";
		m_Text+="  X=0";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="�ظ���";
		m_Text+="\r\n";
		m_Text+="  A=A-X*B��X=X+(A[p]/B[q]-1)*0x100000000**(p-q)��ֱ��A<B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="���У�";
		m_Text+="\r\n";
		m_Text+="  X=C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="ע�⣺";
		m_Text+="\r\n";
		m_Text+="  ���ڴ��������Ϊ0x100000000���ƣ����Զ����������A*0x100000000**k";
		m_Text+="\r\n";
		m_Text+="  ���ȼ��ڽ�A �������еĸ�Ԫ������k λ�����ؼ��㣻ͬ����������ȼ�������";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 7:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="�裺";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)��p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A%B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="��ģ�����̵Ĺ���һ�£�ֻ�����ڲ���Ҫ��¼�̶����Ӽ򵥣�";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="�ظ���";
		m_Text+="\r\n";
		m_Text+="  A=A-(A[p]/B[q]-1)*0x100000000**(p-q)*B��ֱ��A<B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="���У�";
		m_Text+="\r\n";
		m_Text+="  A=C";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 8:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    ��RSA �㷨�У�����Ҫ����֪A��M������£��� B��ʹ�� (A*B)%M=1�����൱��";
		m_Text+="\r\n";
		m_Text+="���B��N����δ֪���Ķ�Ԫһ�β������� A*B-M*N=1������С�����⡣";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ����Բ�������ax-by=1 ����С�����⣬�Ž����ⶼ���й��꾡���о���������";
		m_Text+="\r\n";
		m_Text+="������ŷ������㷨����շת��������й����ؾ��صġ�������һ������ŷ�������";
		m_Text+="\r\n";
		m_Text+="����һ�ֵݹ��㷨���Ƚ�������⣺";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ���磺11x-49y=1����x";
		m_Text+="\r\n";
		m_Text+="    ��a�� 11 x - 49 y = 1    49%11=5 ->";
		m_Text+="\r\n";
		m_Text+="    ��b�� 11 x -  5 y = 1    11%5 =1 ->";
		m_Text+="\r\n";
		m_Text+="    ��c��    x -  5 y = 1";
		m_Text+="\r\n";
		m_Text+="    ��y=0 ���루c����x=1";
		m_Text+="\r\n";
		m_Text+="    ��x=1 ���루b����y=2";
		m_Text+="\r\n";
		m_Text+="    ��y=2 ���루a����x=9";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ͬ���ʹ�õݹ��㷨������� ax-by=1��a��b���ʣ��Ľ⣬ʵ����ͨ����������";
		m_Text+="\r\n";
		m_Text+="���ݹ��㷨ת���ɷǵݹ��㷨�ͱ���˴�����һ����";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 9:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    ��ģ������RSA �����㷨����ֱ�ӵؾ�����RSA �㷨�����ܣ���Կ�����ģ����";
		m_Text+="\r\n";
		m_Text+="��һ���⣬��������ִ���ѧ������˴����Ľ��������ͨ�������Ƚ���ģ���㻯��";
		m_Text+="\r\n";
		m_Text+="Ϊ��ģ���㡣";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ������D=C**15 % N�����ڣ�";
		m_Text+="\r\n";
		m_Text+="        a*b % n = (a % n)*(b % n) % n";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ���ԣ�";
		m_Text+="\r\n";
		m_Text+="        C1=C*C % N       =C**2 % N";
		m_Text+="\r\n";
		m_Text+="        C2=C1*C % N      =C**3 % N";
		m_Text+="\r\n";
		m_Text+="        C3=C2*C2 % N     =C**6 % N";
		m_Text+="\r\n";
		m_Text+="        C4=C3*C % N      =C**7 % N";
		m_Text+="\r\n";
		m_Text+="        C5=C4*C4 % N     =C**14 % N";
		m_Text+="\r\n";
		m_Text+="        C6=C5*C % N      =C**15 % N ";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ����";
		m_Text+="\r\n";
		m_Text+="        ����E=15����ģ����ɷֽ�Ϊ6����ģ����";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ���ɷ������Ϸ������Է��ֶ�������E���ɲ��������㷨����D=C**E % N��";
		m_Text+="\r\n";
		m_Text+="    D=1";
		m_Text+="\r\n";
		m_Text+="    WHILE E>=0";
		m_Text+="\r\n";
		m_Text+="      IF EΪ����";
		m_Text+="\r\n";
		m_Text+="        D=D*C % N";
		m_Text+="\r\n";
		m_Text+="        D=D*D % N";
		m_Text+="\r\n";
		m_Text+="        E=E-1";
		m_Text+="\r\n";
		m_Text+="      IF EΪż��";
		m_Text+="\r\n";
		m_Text+="        D=D*D % N";
		m_Text+="\r\n";
		m_Text+="        E=E/2";
		m_Text+="\r\n";
		m_Text+="    RETURN D";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    �ټ��Է����ᷢ�֣�Ҫ֪��D ��ʱ��� C������Ҫ������E ���м�һ������Ĳ�";
		m_Text+="\r\n";
		m_Text+="����ֻ��Ҫ��֤E �Ķ����Ƹ�λ��0 ����1 �Ϳ����ˣ����Ҵ���������֤�ʹ�������";
		m_Text+="\r\n";
		m_Text+="��֤���У���������������֤���򵥣�";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ��E=Sum[i=0 to n](E[i]*2**i)��0<=E[i]<=1";
		m_Text+="\r\n";
		m_Text+="    D=1";
		m_Text+="\r\n";
		m_Text+="    FOR i=n TO 0";
		m_Text+="\r\n";
		m_Text+="      D=D*D % N";
		m_Text+="\r\n";
		m_Text+="      IF E[i]=1";
		m_Text+="\r\n";
		m_Text+="        D=D*C % N";
		m_Text+="\r\n";
		m_Text+="    RETURN D";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ʣ�µ�������ǳ�ģ�����ˣ�����A*B % N�����A��B ����1024λ�Ĵ������ȼ�";
		m_Text+="\r\n";
		m_Text+="��A*B����% N���ͻ����2048λ���м�������������ö�̬�ڴ���似���ͱ��뽫";
		m_Text+="\r\n";
		m_Text+="���������е�����ռ�����һ������������ɴ������˷ѣ���Ϊ�ھ����������²�";
		m_Text+="\r\n";
		m_Text+="���õ��Ƕ����һ���ռ䣬�����ö�̬�ڴ���似����ʹ�����洢ʧȥ�����Զ�ʹ��";
		m_Text+="\r\n";
		m_Text+="������е�ѭ��������÷ǳ����������Լ������Ҫԭ�����Ҫ�������A*B��";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ���ڣ�";
		m_Text+="\r\n";
		m_Text+="        A*B=A*(Sum[i=0 to n](B[i]*0x100000000**i))";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ���ԣ�";
		m_Text+="\r\n";
		m_Text+="        A*B % N = (Sum[i=0 to n]((A*B[i])*0x100000000**i)) % N";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ������һ��ѭ����ã�";
		m_Text+="\r\n";
		m_Text+="    C=0;";
		m_Text+="\r\n";
		m_Text+="    FOR i=0 to n";
		m_Text+="\r\n";
		m_Text+="      C=C+A*B[i]*0x100000000 % N";
		m_Text+="\r\n";
		m_Text+="    RETURN C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ��ʵ�ϣ���һ���ɸ������㷨�ܹ��������ɶ��ѭ���ĳ�ģ���㣬������ԭ��";
		m_Text+="\r\n";
		m_Text+="�漰�϶������֪ʶ����ʵ�������Ƚ��鷳�����ٶ�������ߣ�������Ҳ���ᳬ��һ";
		m_Text+="\r\n";
		m_Text+="�����������������Ҳ��迼�ǡ�";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 10:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    ����ѧ�����÷���С�����о����˶����������Է�����Ŀǰ�����㷨��������";
		m_Text+="\r\n";
		m_Text+="�ղ����㷨����������£�";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="��1����������M��ʹ��N=(2**r)*M+1";
		m_Text+="\r\n";
		m_Text+="��2��ѡ�������A<N";
		m_Text+="\r\n";
		m_Text+="��3����������i<r����A**((2**i)*M) MOD N = N-1����Nͨ�������A�Ĳ���";
		m_Text+="\r\n";
		m_Text+="��4�����ߣ���A**M MOD N = 1����Nͨ�������A�Ĳ���";
		m_Text+="\r\n";
		m_Text+="��5����Aȡ��ͬ��ֵ��N����5�β��ԣ���ȫ��ͨ�����ж�NΪ����";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ��N ͨ��һ�β��ԣ���N ���������ĸ���Ϊ 25%����N ͨ��t �β��ԣ���N ����";
		m_Text+="\r\n";
		m_Text+="�����ĸ���Ϊ1/4**t����ʵ��ȡt Ϊ5 ʱ��N ���������ĸ���Ϊ 1/128��N Ϊ������";
		m_Text+="\r\n";
		m_Text+="�����Ѿ�����99.99%��";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ��ʵ��Ӧ���У���������300��500��С������N ���в��ԣ�������������ղ���";
		m_Text+="\r\n";
		m_Text+="ͨ���ĸ��ʣ��Ӷ���߲����ٶȡ����������������ʱ��ѡȡ������������ r=0��";
		m_Text+="\r\n";
		m_Text+="���ʡȥ���裨3�� �Ĳ��ԣ���һ����߲����ٶȡ�";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
    case 11:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    ���������������ͨ���ַ�������ɵģ���ʵ�Ϻ�����ʵ�֣����簴��ʮ���Ƹ�";
		m_Text+="\r\n";
		m_Text+="ʽ���д�����";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    ���룺";
		m_Text+="\r\n";
		m_Text+="    X=0";
		m_Text+="\r\n";
		m_Text+="    FOR i=0 TO n";
		m_Text+="\r\n";
		m_Text+="      X=X*10";
		m_Text+="\r\n";
		m_Text+="      X=X+(int)(str[n]-48)";
		m_Text+="\r\n";
		m_Text+="    RETURN X";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    �����";
		m_Text+="\r\n";
		m_Text+="    str=""";
		m_Text+="\r\n";
		m_Text+="    WHILE(X>0)";
		m_Text+="\r\n";
		m_Text+="      str=(char)(X%10-48)+str";
		m_Text+="\r\n";
		m_Text+="    RETURN str ";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	}
	*pResult = 0;
}
