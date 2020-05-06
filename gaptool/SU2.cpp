#include"IGroup.h"
#include<iostream>
#include<complex>
using namespace std;

/*
int getidx(vector<vector<float> > &Arr2,vector<float> &Arr1){
	int n=Arr2.size();
	int m=Arr1.size();	
	for(int i=0;i<n;i++){
		int ni=Arr2[i].size();
		if(ni!=m)
			return -1;
		bool isE=true;
		for(int j=0;j<m;j++){
			if(abs(Arr2[i][j]-Arr1[j])>0.01){
				isE=false;
				break;
			}
		}
		if(isE)
			return i;
	}
	return -1;	
}
*/

///////////////////////////////////////////////////////////////////
// ��غ���
//
///////////////////////////////////////////////////////////////////

// ʸ����ĵ��
inline float Vec3DotP( const vector<float> &v1, const vector<float> &v2 )
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

// ʸ����Ĳ��
inline vector<float> Vec3CrossP( const vector<float> &v1, const vector<float> &v2 )
{
    vector<float> v(3);
	v[0]=v1[1] * v2[2] - v1[2] * v2[1]; 
	v[1]=v1[2] * v2[0] - v1[0] * v2[2];
	v[2]=v1[0] * v2[1] - v1[1] * v2[0];
	return v;
}

// ��Ԫ���˷�
inline vector<float> qmul(const vector<float> &q1, const vector<float> &q2 )
{
	vector<float> xyzw(4);
	vector<float> v1=q1;
	vector<float> v2=q2;
	v1.pop_back();	
	v2.pop_back();
	vector<float> temp=Vec3CrossP(v1,v2);
	xyzw[0]=q1[3]*q2[0]+q1[0]*q2[3]+temp[0];
	xyzw[1]=q1[3]*q2[1]+q1[1]*q2[3]+temp[1];
	xyzw[2]=q1[3]*q2[2]+q1[2]*q2[3]+temp[2];
	xyzw[3]=q1[3]*q2[3]-Vec3DotP(v1,v2);	
	return xyzw;
}

// ��Ԫ���Ĺ���Conjugate
inline vector<float> qconj(const vector<float> &q)
{
	vector<float> xyzw(4);
	xyzw[0] = -q[0];
	xyzw[1] = -q[1];
	xyzw[2] = -q[2];
	xyzw[3] = q[3];
	return xyzw;
}

// ��Ԫ������Inverse,��Ϊq^-1
inline vector<float> qinv(const vector<float> &q)
{
	float f=q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];	
	if(f==0)
		return q;
	float norm2_inv = 1.0 / f;
	vector<float> xyzw(4);
	xyzw[0] = -q[0]*norm2_inv;
	xyzw[1] = -q[1]*norm2_inv;
	xyzw[2] = -q[2]*norm2_inv;
	xyzw[3] = q[3]*norm2_inv;	
	return xyzw;
}

// ����Ԫ������ŷ����������DXû���ṩ���API
inline vector<float> GetEulerAngle(const vector<float> &q)
{
   vector<float> v(3);

   float test = q[1]*q[2] + q[0]*q[3];
   if (test > 0.4999f)
   {
    v[2] = 2.0f * atan2(q[1],q[3]);
    v[1] = 1.570796327f;
    v[0] = 0.0f;
    return v;
   }
   if (test < -0.4999f)
   {
    v[2] = 2.0f * atan2(q[1],q[3]);
    v[1] = -1.570796327f;
    v[0] = 0.0f;
    return v;
   }
   float sqx = q[0] * q[0];
   float sqy = q[1] * q[1];
   float sqz = q[2] * q[2];
   v[2] = atan2(2.0f * q[2] * q[3] - 2.0f * q[1] * q[0], 1.0f - 2.0f * sqz - 2.0f * sqx);
   v[1] = asin(2.0f * test);
   v[0] = atan2(2.0f * q[1] * q[3] - 2.0f * q[2] * q[0], 1.0f - 2.0f * sqy - 2.0f * sqx);
     
   return v;
}

// Convert to Axis/Angles
inline vector<float> Quat2AxisAngle(const vector<float> &q)
{
	vector<float> ret(4);    
	float scale = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
	if(scale==0)
		return ret;
	ret[0] = q[0] / scale;
	ret[1] = q[1] / scale;
	ret[2] = q[2] / scale;
	ret[3] = acos(q[3]) * 2.0;
	return ret;
}

inline vector<float> QuatNormalize(const vector<float>& q)
{
	float f=q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];	
	if(f==0)
		return q;
	float qlength_inv = 1.0 / sqrt(f);
	vector<float> xyzw(4);
	xyzw[0] = q[0]*qlength_inv;
	xyzw[1] = q[1]*qlength_inv;
	xyzw[2] = q[2]*qlength_inv;
	xyzw[3] = q[3]*qlength_inv;
	return xyzw;
}

// Convert from Euler Angles=(pitch,yaw,roll)
inline vector<float> QuatCreateFromEuler(const vector<float>& pyr)
{
	// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	// and multiply those together.
	// the calculation below does the same, just shorter

	double p = pyr[0] * 0.0174532925199433 / 2.0;
	double y = pyr[1] * 0.0174532925199433 / 2.0;
	double r = pyr[2] * 0.0174532925199433 / 2.0;

	double sinp = sin(p);
	double siny = sin(y);
	double sinr = sin(r);
	double cosp = cos(p);
	double cosy = cos(y);
	double cosr = cos(r);

	vector<float> xyzw(4);
    xyzw[0] = sinr * cosp * cosy - cosr * sinp * siny;
	xyzw[1] = cosr * sinp * cosy + sinr * cosp * siny;
	xyzw[2] = cosr * cosp * siny - sinr * sinp * cosy;
	xyzw[3] = cosr * cosp * cosy + sinr * sinp * siny;
	xyzw=QuatNormalize(xyzw);
	
	return xyzw;
}

// �����ǶȺ��ᴴ����Ԫ��
inline vector<float> CQuaternion(const vector<float>& anxi, const float& angle)
{
   vector<float> xyzw(4);
   vector<float> t=anxi;
   float len=sqrt(Vec3DotP(t,t));
   t[0] = t[0]/len;
   t[1] = t[1]/len;
   t[2] = t[2]/len;
   float cosa = cos(angle);
   float sina = sin(angle);
   xyzw[3] = cosa;
   xyzw[0] = sina * t[0];
   xyzw[1] = sina * t[1];
   xyzw[2] = sina * t[2];
   return xyzw;
};

// ����������ת����Ԫ��q, anxi����Ϊ��λ����
inline vector<float> CQuaternion2(const vector<float>& anxi, const float& angle)
{
	float theta_div_2 = (0.5)*angle;
	float sin_theta = sin(theta_div_2);
	vector<float> xyzw(4);
	xyzw[0] = sin_theta * anxi[0];
	xyzw[1] = sin_theta * anxi[1];
	xyzw[2] = sin_theta * anxi[2];
	xyzw[3] = cos( theta_div_2 );
	return xyzw;
}

float g_EulerAngles[][3]=
{
	0,0,0,
	1.570796,0,0,
	0,1.570796,0,
	0,0,1.570796,
	3.14159265,0,0,
	0,3.14159265,0,
	0,0,3.14159265,	
};

float g_AnxiAngles[][4]=
{
	1,0,0,1.570796,
	1,0,0,3.14159265,
	0,1,0,1.570796,
	0,1,0,3.14159265,
	0,0,1,1.570796,
	0,0,1,3.14159265,
};

float g_2T[24][4]=
{
	0,0,0,1,//1
	0,0,0,-1,//-1
	1,0,0,0,//i
	-1,0,0,0,//-i
	0,1,0,0,//j
	0,-1,0,0,//-j
	0,0,1,0,//k
	0,0,-1,0,//-k
	0.5,0.5,0.5,0.5,//q9
	0.5,0.5,0.5,-0.5,//q10
	-0.5,0.5,0.5,0.5,//q11
	-0.5,0.5,0.5,-0.5,//q12
	0.5,-0.5,0.5,0.5,//q13
	0.5,-0.5,0.5,-0.5,//q14
	-0.5,-0.5,0.5,0.5,//q15
	-0.5,-0.5,0.5,-0.5,//q16
	0.5,0.5,-0.5,0.5,//q17
	0.5,0.5,-0.5,-0.5,//q18
	-0.5,0.5,-0.5,0.5,//q19
	-0.5,0.5,-0.5,-0.5,//q20
	0.5,-0.5,-0.5,0.5,//q21
	0.5,-0.5,-0.5,-0.5,//q22
	-0.5,-0.5,-0.5,0.5,//q23
	-0.5,-0.5,-0.5,-0.5,//q24
};

float g_2Ogen[][4]=
{
	1,0,0,0,//i
	0,1,0,0,//j
	0.5,0.5,0.5,0.5,//q9
	0.000000,0.707107,0.707107,0.000000,
};

/*
8�ֵ���LieȺ��һ�㸴����Ⱥ�����⸴����Ⱥ����Ⱥ��������Ⱥ������ʵ����Ⱥ��������Ⱥ��ʵ����Ⱥ������ʵ����Ⱥ���Ĺ�ϵ��
SL(n,R)<SL(n,C)<GL(n,C)
SU(n)<U(n)<GL(n,C)
SO(n,R)<O(n,R)<O(n,C)<GL(n,C)
һ��ʵ����ȺGL(n,R)
��Ԫ�������ĳ˷�ȺH^*
��λ��Ԫ��(x,y,z,t)=((t,-z),(y,-x))���LieȺSU(2),t^2+z^2+y^2+x^2=1��|z_1|^2+|z_2|^2=1
SU(2)��SO(3)��Lie������L(SO(3))=L(SU(2))=A_1
R^4�е�(0,0,0,0)�ľ�����1�ĵ�ļ��Ϲ���S^3�� 
2���Ͼ���u={{a_1+ia_2,b_1+ib_2},{-b_1+ib_2,a_1-ia_2}}��SU(2)��|a_1+ia_2|^2+|b_1+ib_2|^2=1=(a_1)^2+(a_2)^2+(a_3)^2+(a_4)^2�� 
u�����Ͼ���Ķ��壺u*u^(*)=E��det u=1�� 
��ȺU(n)�����Ͼ��󹹳ɵ�����Ⱥ��U(n)��һ���ǵ���ͨ�ǰ뵥���ĵ�LieȺ��
������ʾ���ϱ�ʾ��unitary representation������ʾ����������������Ͼ���D(R)^*D(R)=I��D(R)^*���ΪD(R)�ĸ�����ת�á�
ʵ������ʾ����ʾ������ʵ��������D(R)^*D(R)=I,D(R)^TD(R)=I,detD(R)=��1��ʵ������ʾ��������ʾ��������
ʵ������ʾ����ת�þ�����������
˫������Ⱥ(binary tetrahedral group)2T=SL(2,3)=��ά�Ļ��ĵ�λȺ
2T��1��1��Ԫ��1��2��Ԫ��8��3��Ԫ��6��4��Ԫ��8��6��Ԫ��0��8��Ԫ��0��12��Ԫ��0��24��Ԫ
˫������Ⱥ(binary octahedral group)2O
2O��1��1��Ԫ��1��2��Ԫ��8��3��Ԫ��18��4��Ԫ��8��6��Ԫ��12��8��Ԫ��0��12��Ԫ��0��16��Ԫ��0��24��Ԫ��0��48��Ԫ
˫��ʮ����Ⱥ(binary icosahedral group)2I
*/
// SU(2)��n��������Ⱥ:2T��2O
struct SU2:public IGroup
{
public:
   //  ��̬����
   static bool IsEqual(const vector<float> &t,const vector<float> &m);
   static vector<vector<float> > FG(vector<vector<float> >& gen);   
public:
   // ʵ�ֳ������ķ���
   virtual void printSet();
   virtual void printTable();     
   virtual int mul(int a,int b);
   virtual int size();
   virtual int inv(int a);
   // ���캯��
   SU2(){}// Ĭ�Ϲ��캯��   
   SU2(float* set,int n);
   // ��Ա����
   // ��Ա���� 
   vector<vector<float> > s_Arr; 
};

bool SU2::IsEqual(const vector<float> &t,const vector<float> &m){
	int nt=t.size();
	int nm=m.size();
	if(nt!=nm)
		return false;
	for(int i=0;i<nt;i++){
			if(abs(t[i]-m[i])>0.01)
				return false;
	}
	return true;
}

vector<vector<float> > SU2::FG(vector<vector<float> >& gen)
{
	vector<vector<float> > S;
	int n=gen.size();
	vector<float> m0(4,0);
	m0[3]=1;
	S.push_back(m0);
	int R=gen.size();
	for(int i=0;i<R;i++){
		if(IsEqual(gen[i],m0))
			continue;
		S.push_back(gen[i]);
	}
	int cnt=R+1;
	int cnt1=R+1;
	do{
		cnt=S.size();
		for(int i=1;i<cnt;i++){
			for(int j=1;j<cnt;j++){
				vector<float> IJ=qmul(S[i],S[j]);
				vector<vector<float> >::iterator p=std::find_if(S.begin(),S.end(),[IJ](vector<float>& m)->bool{return IsEqual(m,IJ);});
				if(p==S.end()){
					S.push_back(IJ);
				}
			}
		}
		cnt1=S.size();
	}while(cnt1>cnt);	
   return S;
}

SU2::SU2(float* set,int n)
{
	vector<float> v(4);
	for(int i=0;i<n;i++)
	{
		v[0]=*(set+4*i);
		v[1]=*(set+4*i+1);
		v[2]=*(set+4*i+2);
		v[3]=*(set+4*i+3);
		s_Arr.push_back(v);						
	}
}

#define R2D(x) ((int)(x*180.0/3.141592654)) 
void SU2::printSet()
{
	int ID=IdGroup(this);	
	printf("SU(2)����ȺGAP[%d,%d]:\n",size(),ID);  
	for(int i=0;i<size();i++){
		vector<float> v1=qinv(s_Arr[i]);
		int i1=getidx(s_Arr,v1);
		vector<float> arg=GetEulerAngle(s_Arr[i]);
		vector<float> arg1=GetEulerAngle(v1);		
		printf("%d->[[%.3f,%.3f,%.3f],%.3f],EulerAngle=(%d,%d,%d)=>%d->[[%.3f,%.3f,%.3f],%f]],EulerAngle=(%d,%d,%d)\n", \
		i,s_Arr[i][0],s_Arr[i][1],s_Arr[i][2],s_Arr[i][3],R2D(arg[0]),R2D(arg[1]),R2D(arg[2]), \
		i1,v1[0],v1[1],v1[2],v1[3],R2D(arg1[0]),R2D(arg1[1]),R2D(arg1[2]));
	}
}
#undef R2D

void SU2::printTable()
{
   int ID=IdGroup(this);	
   printf("SU(2)����ȺGAP[%d,%d]:\n",size(),ID);     
   printGroup(this);
}

int SU2::mul(int a,int b)
{
	vector<float> A=s_Arr[a];
	vector<float> B=s_Arr[b];
	vector<float> C=qmul(A,B);
	int idx=getidx(s_Arr,C);
	return idx;
}

int SU2::inv(int a)
{
	vector<float> v=qinv(s_Arr[a]);
	int idx=getidx(s_Arr,v);
	return idx;
}

int SU2::size()
{
	return s_Arr.size();	
}

int main()
{
	SU2 _2T(&g_2T[0][0],sizeof(g_2T)/sizeof(g_2T[0]));
	_2T.printSet();
	_2T.printTable();
	
	vector<vector<float> > vv;
	vector<float> v(4);
    int n=sizeof(g_2Ogen)/sizeof(g_2Ogen[0]);
	for(int i=0;i<n;i++)
	{
		v[0]=g_2Ogen[i][0];
		v[1]=g_2Ogen[i][1];
		v[2]=g_2Ogen[i][2];
		v[3]=g_2Ogen[i][3];
		vv.push_back(v);						
	}
	SU2 _2O;
	_2O.s_Arr=SU2::FG(vv);
	_2O.printSet();
	_2O.printTable();
	
	if(1)
	{
        printf("test QuatCreateFromEuler��GetEulerAngle��Quat2AxisAngle\n");
		vector<vector<float> > s_Arr1; 
		int n=sizeof(g_EulerAngles)/sizeof(g_EulerAngles[0]);
		vector<float> v(n);
		for(int i=0;i<n;i++)
		{
			v[0]=g_EulerAngles[i][0];
			v[1]=g_EulerAngles[i][1];
			v[2]=g_EulerAngles[i][2];
			s_Arr1.push_back(v);
            vector<float> v1=QuatCreateFromEuler(v);
			vector<float> arg=GetEulerAngle(v1);
			vector<float> v0=Quat2AxisAngle(v1);			
			printf("EulerAngle=(%.3f,%.3f,%.3f)->[[%.3f,%.3f,%.3f],%.3f],EulerAngle=(%.3f,%.3f,%.3f),AnxiAngles=[(%.3f,%.3f,%.3f),%.3f]\n", \
			v[0],v[1],v[2],v1[0],v1[1],v1[2],v1[3],arg[0],arg[1],arg[2],v0[0],v0[1],v0[2],v0[3]);
		}
	}
	
	if(1)
	{
        printf("test CQuaternion��GetEulerAngle��Quat2AxisAngle\n");
		vector<vector<float> > s_Arr1; 
		int n=sizeof(g_AnxiAngles)/sizeof(g_AnxiAngles[0]);
		vector<float> v(n);
		for(int i=0;i<n;i++)
		{
			v[0]=g_AnxiAngles[i][0];
			v[1]=g_AnxiAngles[i][1];
			v[2]=g_AnxiAngles[i][2];
			s_Arr1.push_back(v);
            vector<float> v1=CQuaternion(v,g_AnxiAngles[i][3]);
			vector<float> arg=GetEulerAngle(v1);
			vector<float> v0=Quat2AxisAngle(v1);
			printf("AnxiAngles=[(%.3f,%.3f,%.3f),%.3f]->[[%.3f,%.3f,%.3f],%.3f],EulerAngle=(%.3f,%.3f,%.3f),AnxiAngles=[(%.3f,%.3f,%.3f),%.3f]\n", \
			v[0],v[1],v[2],g_AnxiAngles[i][3],v1[0],v1[1],v1[2],v1[3],arg[0],arg[1],arg[2],v0[0],v0[1],v0[2],v0[3]);
		}
	}	
	
	if(1)
	{
        printf("test CQuaternion2��GetEulerAngle��Quat2AxisAngle\n");
		vector<vector<float> > s_Arr1; 
		int n=sizeof(g_AnxiAngles)/sizeof(g_AnxiAngles[0]);
		vector<float> v(n);
		for(int i=0;i<n;i++)
		{
			v[0]=g_AnxiAngles[i][0];
			v[1]=g_AnxiAngles[i][1];
			v[2]=g_AnxiAngles[i][2];
			s_Arr1.push_back(v);
            vector<float> v1=CQuaternion2(v,g_AnxiAngles[i][3]);
			vector<float> arg=GetEulerAngle(v1);
			vector<float> v0=Quat2AxisAngle(v1);
			printf("AnxiAngles=[(%.3f,%.3f,%.3f),%.3f]->[[%.3f,%.3f,%.3f],%.3f],EulerAngle=(%.3f,%.3f,%.3f),AnxiAngles=[(%.3f,%.3f,%.3f),%.3f]\n", \
			v[0],v[1],v[2],g_AnxiAngles[i][3],v1[0],v1[1],v1[2],v1[3],arg[0],arg[1],arg[2],v0[0],v0[1],v0[2],v0[3]);
		}
	}	
	
	return 0;
}


