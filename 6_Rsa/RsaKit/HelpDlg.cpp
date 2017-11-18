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
	TreeCtrlItem.item.pszText = _T("原理介绍");
	TreeCtrlItem.item.lParam = 0;
	HTREEITEM hTreeItem1 = m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("大数存储");
	TreeCtrlItem.item.lParam = 2;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("加法");
	TreeCtrlItem.item.lParam = 3;
	m_Tree.InsertItem(&TreeCtrlItem);  
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("减法");
	TreeCtrlItem.item.lParam = 4;
	m_Tree.InsertItem(&TreeCtrlItem); 

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("乘法");
	TreeCtrlItem.item.lParam = 5;
	m_Tree.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("除法");
	TreeCtrlItem.item.lParam = 6;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("取模");
	TreeCtrlItem.item.lParam = 7;
	m_Tree.InsertItem(&TreeCtrlItem); 
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("二元一次方程");
	TreeCtrlItem.item.lParam = 8;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("幂模运算");
	TreeCtrlItem.item.lParam = 9;
	m_Tree.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("素数测试");
	TreeCtrlItem.item.lParam = 10;
	m_Tree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = _T("输入输出");
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
		m_Text+="    RSA 依赖大数运算，目前主流RSA 算法都建立在512 到1024位的大数运算之上。";
		m_Text+="\r\n";
		m_Text+="而大多数的编译器只能支持到64位的整数运算，即我们在运算中所使用的整数必须小";
		m_Text+="\r\n";
		m_Text+="于等于64位，即：0xffffffffffffffff，也就是18446744073709551615，这远远达不";
		m_Text+="\r\n";
		m_Text+="到RSA 的需要，于是需要专门建立大数运算库来解决这一问题。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    最简单的办法是将大数当作数组进行处理，也就是将大数用0—9这十个数字组成";
		m_Text+="\r\n";
		m_Text+="的数组进行表示，然后模拟人们手工进行“竖式计算”的过程编写其加减乘除函数。";
		m_Text+="\r\n";
		m_Text+="但是这样做效率很低，因为二进制为1024位的大数其十进制也有三百多位，对于任何";
		m_Text+="\r\n";
		m_Text+="一种运算，都需要在两个有数百个元素的数组空间上做多重循环，还需要许多额外的";
		m_Text+="\r\n";
		m_Text+="空间存放计算的进退位标志及中间结果。另外，对于某些特殊的运算而言，采用二进";
		m_Text+="\r\n";
		m_Text+="制会使计算过程大大简化，这种大数表示方法转化成二进制显然非常麻烦，所以在某";
		m_Text+="\r\n";
		m_Text+="些实例中则干脆采用了二进制数组的方法来记录大数，这样效率就更低了。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    一个有效的改进方法是将大数表示为一个n 进制数组，对于目前的32位系统而言";
		m_Text+="\r\n";
		m_Text+="n 可以取值为2 的32次方，即0x10000000，假如将一个二进制为1024位的大数转化成";
		m_Text+="\r\n";
		m_Text+="0x10000000进制，它就变成了32位，而每一位的取值范围就不是二进制的0—1或十进";
		m_Text+="\r\n";
		m_Text+="制的0—9，而是0-0xffffffff，我们正好可以用一个无符号长整数来表示这一数值。";
		m_Text+="\r\n";
		m_Text+="所以1024位的大数就是一个有32个元素的unsigned long数组，针对unsigned long数";
		m_Text+="\r\n";
		m_Text+="组进行各种运算所需的循环规模至多32次而已。而且0x100000000 进制与二进制，对";
		m_Text+="\r\n";
		m_Text+="于计算机来说，几乎是一回事，转换非常容易。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    例如大数18446744073709551615，等于 ffffffff ffffffff，就相当于十进制的";
		m_Text+="\r\n";
		m_Text+="99：有两位，每位都是ffffffff。而18446744073709551616 等于00000001 00000000";
		m_Text+="\r\n";
		m_Text+="00000000，就相当于十进制的100：有三位，第一位是1 ，其它两位是0，如此等等。";
		m_Text+="\r\n";
		m_Text+="在实际应用中，“数字”数组的排列顺序采用低位在前高位在后的方式，这样，大数";
		m_Text+="\r\n";
		m_Text+="A 就可以方便地用数学表达式来表示其值：A=Sum[i=0 to n](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="（其中Sum 表示求和，A[i]表示用以记录A 的数组的第i 个元素，**表示乘方）。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    任何整数运算最终都能分解成数字与数字之间的运算，在0x100000000 进制下其";
		m_Text+="\r\n";
		m_Text+="“数字”最大达到0xffffffff，其数字与数字之间的运算，结果也必然超出了目前32";
		m_Text+="\r\n";
		m_Text+="系统的字长。在VC++中，存在一个__int64 类型可以处理64位的整数，所以不用担心";
		m_Text+="\r\n";
		m_Text+="这一问题，而在其它编译系统中如果不存在64位整形，就需要采用更小的进制方式来";
		m_Text+="\r\n";
		m_Text+="存储大数，例如WORD类型（16位）可以用来表示0x10000 进制，但效率更高的办法还";
		m_Text+="\r\n";
		m_Text+="是采用32位的DWORD 类型，只不过将0x100000000 进制改成0x40000000进制，这样两";
		m_Text+="\r\n";
		m_Text+="个数字进行四则运算的最大结果为 0x3fffffff * 0x3fffffff，小于0xffffffff，只";
		m_Text+="\r\n";
		m_Text+="是不能简单地用高位低位来将运算结果拆分成两个“数字”。";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 3:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="设：";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)，p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A+B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="显然：";
		m_Text+="\r\n";
		m_Text+="  C[i]不是简单地等于A[i]+B[i]，因为如果C[i]>0xffffffff就需要进位，当然计算";
		m_Text+="\r\n";
		m_Text+="  C[i-1]时也可能产生了进位，所以计算C[i]时还要加上上次的进位值。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="如果用carry[i]记录每次的进位则有：";
		m_Text+="\r\n";
		m_Text+="  C[i]=A[i]+B[i]+carry[i-1]-carry[i]*0x100000000";
		m_Text+="\r\n";
		m_Text+="  其中carry[-1]=0";
		m_Text+="\r\n";
		m_Text+="  若A[i]+B[i]+carry[i-1]>0xffffffff，则carry[i]=1；反之则carry[i]=0";
		m_Text+="\r\n";
		m_Text+="  若carry[p]=0，则n=p；反之则n=p+1";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 4:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="设：";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)，p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A-B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="显然：";
		m_Text+="\r\n";
		m_Text+="  C[i]不是简单地等于A[i]-B[i]，因为如果A[i]<B[i]就需要借位，当然计算";
		m_Text+="\r\n";
		m_Text+="  C[i-1]时也可能产生了借位，所以计算C[i]时还要减去上次的借位值。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="如果用carry[i]记录每次的借位则有：";
		m_Text+="\r\n";
		m_Text+="  C[i]=A[i]+carry[i]*0x100000000-B[i]-carry[i-1]";
		m_Text+="\r\n";
		m_Text+="  其中carry[-1]=0";
		m_Text+="\r\n";
		m_Text+="  若A[i]>B[i]则carry[i]=0；反之则carry[i]=1";
		m_Text+="\r\n";
		m_Text+="  若C[p]=0，则n=p-1；反之则n=p";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 5:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="设：";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)，p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A*B";
		m_Text+="\r\n";
		m_Text+="";
		m_Text+="\r\n";
		m_Text+="显然：";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to q](A*B[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  而(A*B[i]*100000000**i)=Sum[j=0 to p](A[j]*B[i]*0x100000000**(i+j))";
		m_Text+="\r\n";
		m_Text+="  所以C=Sum[i=0 to q](Sum[j=0 to p](A[j]*B[i]*0x100000000**(i+j)))";
		m_Text+="\r\n";
		m_Text+="";
		m_Text+="\r\n";
		m_Text+="因此：";
		m_Text+="\r\n";
		m_Text+="  C[i]=Sum[j=0 to q](A[i-j]*B[j])+carry[i-1]-carry[i]*0x100000000";
		m_Text+="\r\n";
		m_Text+="  其中carry[-1]=0";
		m_Text+="\r\n";
		m_Text+="  carry[i]=(Sum[j=0 to q](A[i-j]*B[j])+carry[i-1])/0x100000000";
		m_Text+="\r\n";
		m_Text+="  n=p+q-1，若carry[n]>0，则n=n+1，C[n]=carry";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 6:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="设：";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)，p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A/B";
		m_Text+="\r\n";
		m_Text+="";
		m_Text+="\r\n";
		m_Text+="由于无法将B 对A “试商”，我们只能转换成B[q]对A[p]的试商来得到一个近似值，";
		m_Text+="\r\n";
		m_Text+="所以我们不能够直接计算C。但是，我们可以一步一步地逼近C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="显然：";
		m_Text+="\r\n";
		m_Text+="  (A[p]/B[q]-1)*0x100000000**(p-q)<C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="令：";
		m_Text+="\r\n";
		m_Text+="  X=0";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="重复：";
		m_Text+="\r\n";
		m_Text+="  A=A-X*B，X=X+(A[p]/B[q]-1)*0x100000000**(p-q)，直到A<B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="则有：";
		m_Text+="\r\n";
		m_Text+="  X=C";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="注意：";
		m_Text+="\r\n";
		m_Text+="  由于大数可理解为0x100000000进制，所以对于任意大数A*0x100000000**k";
		m_Text+="\r\n";
		m_Text+="  都等价于将A 的数组中的各元素左移k 位，不必计算；同样，除法则等价于右移";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 7:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="设：";
		m_Text+="\r\n";
		m_Text+="  A=Sum[i=0 to p](A[i]*0x100000000**i)";
		m_Text+="\r\n";
		m_Text+="  B=Sum[i=0 to q](B[i]*0x100000000**i)，p>=q";
		m_Text+="\r\n";
		m_Text+="  C=Sum[i=0 to n](C[i]*0x100000000**i)=A%B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="求模与求商的过程一致，只是由于不需要记录商而更加简单：";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="重复：";
		m_Text+="\r\n";
		m_Text+="  A=A-(A[p]/B[q]-1)*0x100000000**(p-q)*B，直到A<B";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="则有：";
		m_Text+="\r\n";
		m_Text+="  A=C";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 8:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    在RSA 算法中，往往要在已知A、M的情况下，求 B，使得 (A*B)%M=1。即相当于";
		m_Text+="\r\n";
		m_Text+="求解B、N都是未知数的二元一次不定方程 A*B-M*N=1，的最小整数解。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    而针对不定方程ax-by=1 的最小整数解，古今中外都进行过详尽的研究，西方有";
		m_Text+="\r\n";
		m_Text+="著名的欧几里德算法，即辗转相除法，中国有秦九韶的“大衍求一术”。欧几里德算";
		m_Text+="\r\n";
		m_Text+="法是一种递归算法，比较容易理解：";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    例如：11x-49y=1，求x";
		m_Text+="\r\n";
		m_Text+="    （a） 11 x - 49 y = 1    49%11=5 ->";
		m_Text+="\r\n";
		m_Text+="    （b） 11 x -  5 y = 1    11%5 =1 ->";
		m_Text+="\r\n";
		m_Text+="    （c）    x -  5 y = 1";
		m_Text+="\r\n";
		m_Text+="    令y=0 代入（c）得x=1";
		m_Text+="\r\n";
		m_Text+="    令x=1 代入（b）得y=2";
		m_Text+="\r\n";
		m_Text+="    令y=2 代入（a）得x=9";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    同理可使用递归算法求得任意 ax-by=1（a、b互质）的解，实际上通过分析归纳";
		m_Text+="\r\n";
		m_Text+="将递归算法转换成非递归算法就变成了大衍求一术。";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 9:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    幂模运算是RSA 核心算法，最直接地决定了RSA 算法的性能，针对快速幂模运算";
		m_Text+="\r\n";
		m_Text+="这一课题，许多西方现代数学家提出了大量的解决方案。通常都是先将幂模运算化简";
		m_Text+="\r\n";
		m_Text+="为乘模运算。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    例如求D=C**15 % N，由于：";
		m_Text+="\r\n";
		m_Text+="        a*b % n = (a % n)*(b % n) % n";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    所以：";
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
		m_Text+="    即：";
		m_Text+="\r\n";
		m_Text+="        对于E=15的幂模运算可分解为6个乘模运算";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    归纳分析以上方法可以发现对于任意E，可采用以下算法计算D=C**E % N：";
		m_Text+="\r\n";
		m_Text+="    D=1";
		m_Text+="\r\n";
		m_Text+="    WHILE E>=0";
		m_Text+="\r\n";
		m_Text+="      IF E为奇数";
		m_Text+="\r\n";
		m_Text+="        D=D*C % N";
		m_Text+="\r\n";
		m_Text+="        D=D*D % N";
		m_Text+="\r\n";
		m_Text+="        E=E-1";
		m_Text+="\r\n";
		m_Text+="      IF E为偶数";
		m_Text+="\r\n";
		m_Text+="        D=D*D % N";
		m_Text+="\r\n";
		m_Text+="        E=E/2";
		m_Text+="\r\n";
		m_Text+="    RETURN D";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    再加以分析会发现，要知道D 何时需乘 C，不需要反复对E 进行减一或除二的操";
		m_Text+="\r\n";
		m_Text+="作，只需要验证E 的二进制各位是0 还是1 就可以了，而且从左至右验证和从右至左";
		m_Text+="\r\n";
		m_Text+="验证都行，反而从左至右验证更简单：";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    若E=Sum[i=0 to n](E[i]*2**i)，0<=E[i]<=1";
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
		m_Text+="    剩下的问题就是乘模运算了，对于A*B % N，如果A、B 都是1024位的大数，先计";
		m_Text+="\r\n";
		m_Text+="算A*B，再% N，就会产生2048位的中间结果，如果不采用动态内存分配技术就必须将";
		m_Text+="\r\n";
		m_Text+="大数定义中的数组空间增加一倍，这样会造成大量的浪费，因为在绝大多数情况下不";
		m_Text+="\r\n";
		m_Text+="会用到那额外的一倍空间，而采用动态内存分配技术会使大数存储失去连续性而使运";
		m_Text+="\r\n";
		m_Text+="算过程中的循环操作变得非常繁琐。所以计算的首要原则就是要避免计算A*B。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    由于：";
		m_Text+="\r\n";
		m_Text+="        A*B=A*(Sum[i=0 to n](B[i]*0x100000000**i))";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    所以：";
		m_Text+="\r\n";
		m_Text+="        A*B % N = (Sum[i=0 to n]((A*B[i])*0x100000000**i)) % N";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    可以用一个循环求得：";
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
		m_Text+="    事实上，有一种蒙哥马利算法能够更快地完成多次循环的乘模运算，但是其原理";
		m_Text+="\r\n";
		m_Text+="涉及较多的数论知识，且实现起来比较麻烦，对速度虽有提高，经测试也不会超过一";
		m_Text+="\r\n";
		m_Text+="个数量级，所以暂且不予考虑。";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
	case 10:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    数论学家利用费马小定理研究出了多种素数测试方法，目前最快的算法是拉宾米";
		m_Text+="\r\n";
		m_Text+="勒测试算法，其过程如下：";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="（1）计算奇数M，使得N=(2**r)*M+1";
		m_Text+="\r\n";
		m_Text+="（2）选择随机数A<N";
		m_Text+="\r\n";
		m_Text+="（3）对于任意i<r，若A**((2**i)*M) MOD N = N-1，则N通过随机数A的测试";
		m_Text+="\r\n";
		m_Text+="（4）或者，若A**M MOD N = 1，则N通过随机数A的测试";
		m_Text+="\r\n";
		m_Text+="（5）让A取不同的值对N进行5次测试，若全部通过则判定N为素数";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    若N 通过一次测试，则N 不是素数的概率为 25%，若N 通过t 次测试，则N 不是";
		m_Text+="\r\n";
		m_Text+="素数的概率为1/4**t。事实上取t 为5 时，N 不是素数的概率为 1/128，N 为素数的";
		m_Text+="\r\n";
		m_Text+="概率已经大于99.99%。";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    在实际应用中，可首先用300—500个小素数对N 进行测试，以提高拉宾米勒测试";
		m_Text+="\r\n";
		m_Text+="通过的概率，从而提高测试速度。而在生成随机素数时，选取的随机数最好让 r=0，";
		m_Text+="\r\n";
		m_Text+="则可省去步骤（3） 的测试，进一步提高测试速度。";
		m_Text+="\r\n";
		m_Text+="\r\n";
        m_Edit.SetWindowText(m_Text);
		break;
    case 11:
        m_Text="";
		m_Text+="\r\n";
		m_Text+="    大数的输入输出是通过字符串来完成的，事实上很容易实现，例如按照十进制格";
		m_Text+="\r\n";
		m_Text+="式进行处理，则：";
		m_Text+="\r\n";
		m_Text+="\r\n";
		m_Text+="    输入：";
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
		m_Text+="    输出：";
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
