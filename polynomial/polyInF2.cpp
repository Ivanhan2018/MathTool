#include "polyInF2.h"

vector<polyInF2>				polyInF2::irreducible;

polyInF2::polyInF2() {

	coef.clear();
	coef.insert(coef.begin(), 0);	
}

polyInF2::polyInF2(const polyInF2 & src) {

	coef.clear();
	coef.reserve(src.coef.size());
	coef.insert(coef.end(), src.coef.begin(), src.coef.end());
	refresh();
}

polyInF2::polyInF2(const string & str) {	

	bool				isBegin = false;
	string::size_type	i = 0;
	const string::size_type		len = str.size();
		
	coef.clear();
	coef.reserve(len);
	while (i < len) {
		char ch = str[i++];
		if (!isBegin) {
			if (ch == '0') {				
				continue;			// 忽略前导零
			} else {
				isBegin = true;		// 开始读取
			}
		}
		if (ch == '0') {
			coef.push_back(0);
		} else if (ch == '1') {
			coef.push_back(1);
		} else {
			coef.clear();
			cerr<<"Invalid input sequence!"<<endl;
		}
	}
	reverse(coef.begin(), coef.end());
	refresh();
}

polyInF2::~polyInF2() {

}

void polyInF2::refresh() {

	while (coef.size() && *(coef.rbegin()) == 0) {	// 清除高位无效零
		coef.pop_back();
	}
	if (coef.empty()) {	// 补零
		coef.push_back(0);
	}
}


polyInF2 & polyInF2::operator = (const polyInF2 & rval) {

	coef.clear();
	coef.reserve(rval.coef.size());
	coef.insert(coef.end(), rval.coef.begin(), rval.coef.end());
	refresh();
	return *this;
}

bool polyInF2::operator == (const polyInF2 & rval) const {

	return rval.coef == coef;
}
	
polyInF2 polyInF2::operator + (const polyInF2 & rval) const{

	polyInF2			RetVal(*this);	
	// a = a + b
	vector<int>			&vec_a = RetVal.coef;
	const vector<int>	&vec_b = rval.coef;
	const size_t		a_size = vec_a.size();
	const size_t		b_size = vec_b.size();
	const size_t		min_size = min(a_size, b_size);
	
	for (size_t idx = 0; idx < min_size; idx++) {	// 模2加
		if (vec_b[idx]) {
			vec_a[idx] = 1 - vec_a[idx];
		}
	}
	if (b_size > min_size) {	// 若b比a长，附加a的高位
		vec_a.insert(vec_a.end(), vec_b.begin() + min_size, vec_b.end());
	}
	RetVal.refresh();
	return RetVal;
}
	
polyInF2 polyInF2::operator - (const polyInF2 & rval) const {

	return (*this) + rval;
}

polyInF2 polyInF2::operator * (const polyInF2 & rval) const {

	polyInF2				RetVal;
	// r = a * b
	vector<int>			&vec_r = RetVal.coef;
	const vector<int>	&vec_a = coef;
	const vector<int>	&vec_b = rval.coef;
	const size_t		a_size = vec_a.size();
	const size_t		b_size = vec_b.size();	

	vec_r.resize(a_size + b_size - 1);	// 预留结果数组
	for (size_t b_idx = 0; b_idx < b_size; b_idx++) {	// 位移累加
		if (!vec_b[b_idx])
			continue;
		for (size_t a_idx = 0; a_idx < a_size; a_idx++) {
			if (vec_a[a_idx]) {
				vec_r[a_idx + b_idx] = 1 - vec_r[a_idx + b_idx];
			}
		}
	}
	RetVal.refresh();
	return RetVal;
}

polyInF2 polyInF2::operator % (const polyInF2 & rval) const {

	if (rval.isZero()) {
		cerr<<"Divided by ZERO!"<<endl;
		return polyInF2();
	}

	polyInF2				RetVal(*this);
	// a = a % b
	vector<int>			&vec_a = RetVal.coef;
	const vector<int>	vec_b(rval.coef.begin(), rval.coef.end());
	const size_t		b_size = vec_b.size();
	size_t				a_size = vec_a.size();

	while (a_size >= b_size) {	// 逐位除法
		a_size--;
		if (vec_a[a_size] == 1) {	// 测试最高位，位1时可除
			for (size_t i = 1; i < b_size; i++) {	// 模2加，最高位不必参与运算
				if (vec_b[b_size - 1 - i]) {
					vec_a[a_size - i] = 1 - vec_a[a_size - i];
				}				
			}
		}
		vec_a.pop_back();	// 删除最高位
    }
	RetVal.refresh();
	return RetVal;
}

polyInF2 polyInF2::operator / (const polyInF2 & rval) const {

	if (rval.isZero()) {
		cerr<<"Divided by ZERO!"<<endl;
		return polyInF2();
	}

	polyInF2				RetVal(*this);
	// a = a / b
	vector<int>			&vec_a = RetVal.coef;
	const vector<int>	vec_b(rval.coef.begin(), rval.coef.end());	
	const size_t		b_size = vec_b.size();

	size_t				a_size = vec_a.size();
	while (a_size >= b_size) {	// 逐位除法
		a_size--;
		if (vec_a[a_size] == 1) {	// 测试最高位，位1时可除
			for (size_t i = 1; i < b_size; i++) {	// 模2加，最高位不必参与运算留作结果
				if (vec_b[b_size - 1 - i]) {
					vec_a[a_size - i] = 1 - vec_a[a_size - i];
				}
			}			
		}
    }
	if (vec_a.size() >= b_size) {
		vec_a.erase(vec_a.begin(), vec_a.begin() + b_size - 1);	// 删除商中无用的低位
	} else {
		vec_a.clear();
	}
	RetVal.refresh();
	return RetVal;
}

bool polyInF2::isZero() const {

	return (coef.size() == 1 && coef[0] == 0);
}

bool polyInF2::isOne() const {

	return (coef.size() == 1 && coef[0] == 1);
}

bool polyInF2::isIrreducible() const {

	if (isZero() || isOne()) {	
		return true;
	}

	const size_t	m = coef.size() - 1;
	// 生成次数不高于 [m/2] 的不可约多项式表	
	polyInF2::generateIrreducible(m/2);
	// 测试是否为不可约多项式
	size_t		i = 0;
	while (i < polyInF2::irreducible.size()) {
		if (polyInF2::irreducible[i].coef.size() > (m/2 + 1))
			break;
		if (((*this) % polyInF2::irreducible[i]).isZero()) {
			//cerr<<"Debug: "<<(*this)<<" is reducible"<<endl;
			//cerr<<"Debug: "<<(*this)<<" == 0 ( mod "<<polyInF2::irreducible[i]<<" )"<<endl;
			return false;
		}
		i++;
	}
	return true;
}

bool polyInF2::isPrimitive_step2() const {

	polyInF2			f;
	const size_t	m = coef.size() - 1;

	// 检测可整除 x^n + 1 ( n = 2^m - 1 )
	if (m > 8 * sizeof(size_t)) {
		cerr<<"2^"<<(unsigned long)m<<"-1 is out of range!"<<endl;
		return false;
	}
	const size_t	max_exp = (0x1 << m) - 1;
	size_t			n;		
	n = max_exp;
	f.coef.clear();
	f.coef.resize(n + 1, 0);	
	f.coef[0] = 1;
	f.coef[n] = 1;
	if (! (f % (*this)).isZero()) {
		//cerr<<"Debug: "<<(*this)<<" is not primitive, because"<<endl;
		//cerr<<"Debug: "<<f<<" == "<<(f % (*this))<<" ( mod "<<(*this)<<" )"<<endl;
		return false;
	}

	// 检测不可整除 x^n + 1 ( m <= n < 2^m - 1 )

	// 方法一：高位高位
	// m = 10, 1202 ms
	// m = 11, 11256 ms
	// m = 12, 54588 ms

	//while (n >= m) {
	//	f.coef.pop_back();
	//	f.coef[--n] = 1;		
	//	if ((f % *this).isZero()) {
	//		//cerr<<"Debug: "<<(*this)<<" is not primitive, because"<<endl;
	//		//cerr<<"Debug: "<<f<<" == 0 ( mod "<<(*this)<<" )"<<endl;
	//		return false;
	//	}
	//}

	// 方法二：低位开始
	// m = 10, 991 ms
	// m = 11, 11196 ms
	// m = 12, 40879 ms

	n = m;
	f.coef.clear();			
	f.coef.resize(n + 1, 0);
	f.coef.reserve(max_exp);
	f.coef[0] = 1;
	f.coef[n] = 0;	
	while (n < max_exp) {
		if ((f % *this).isZero()) {
			//cerr<<"Debug: "<<(*this)<<" is not primitive, because"<<endl;
			//cerr<<"Debug: "<<f<<" == 0 ( mod "<<(*this)<<" )"<<endl;
			return false;
		}
		f.coef[n++] = 0;
		f.coef.push_back(1);
	}
	return true;
}

bool polyInF2::isPrimitive() const {
		
	if (isZero() || isOne()) {	
		return false;
	}
	
	if (!(*this).isIrreducible()) {
		//cerr<<"Debug: "<<(*this)<<" is not primitive, because it is NOT irreducible"<<endl;
		return false;
	}

	return isPrimitive_step2();
}

polyInF2 polyInF2::inverse (const polyInF2 & p) const {

	if (!p.isPrimitive()) {
		cerr<<"Invalid primitive!"<<endl;
		return polyInF2();
	}

	// a * b = 1 ( mod p )
	const polyInF2			a = (*this) % p;	
	const size_t		p_size = p.coef.size();
	const size_t		a_size = a.coef.size();
	const size_t		b_max_size = p_size - 1;	

	if (a.isZero()) {
		cerr<<*this<<" %  "<<p<<" = 0"<<endl;
		cerr<<"No solution!"<<endl;
		return polyInF2();
	}

	// 生成系数查找表
	vector<polyInF2>		Table;
	polyInF2				f;
	size_t				idx;

	f.coef.clear();
	f.coef.reserve(b_max_size + a_size - 1);
	f.coef.push_back(1);
	Table.clear();
	Table.reserve(b_max_size + a_size - 1);		
	idx = 0;
	while (idx < b_max_size) {
		Table.push_back(f);
		//cout<<Table[idx]<<endl;		
		f.coef[idx++] = 0;
		f.coef.push_back(1);

	}
	while (idx < b_max_size + a_size - 1) {
		Table.push_back(f % p);
		//cout<<Table[idx]<<endl;
		f.coef[idx++] = 0;
		f.coef.push_back(1);
	}

	// 生成方程组
	// 最后一位留作高斯消元标志位，倒数第二位为方程组右矩阵
	vector<vector<int> >		Matrix(b_max_size, vector<int>(b_max_size + 2, 0));
	for (size_t a_idx = 0; a_idx < a_size; a_idx++) {	// 逐位乘法
		if (a.coef[a_idx] == 0)
			continue;
		for (size_t x_idx = a_idx;x_idx < a_idx + b_max_size; x_idx++) {	// 将系数查找表模2加到方程组
			//cout<<"a_"<<x_idx - a_idx<<" * "<<Table[x_idx]<<endl;
			for (idx = 0; idx < Table[x_idx].coef.size(); idx++) {
				//cout<<"\tMatrix["<< idx <<"]["<< x_idx - a_idx <<"] add "<< Table[x_idx].coef[idx] <<endl;
				Matrix[idx][x_idx - a_idx] += Table[x_idx].coef[idx];
				Matrix[idx][x_idx - a_idx] %= 2;				
			}
		}
	}
	Matrix[0][b_max_size] = 1;	// 填充右矩阵

	//cout<<"Matrix:"<<endl;
	//for (size_t i = 0; i < b_max_size; i++) {
	//	cout<<"\t";
	//	for (size_t j = 0; j < b_max_size + 1; j++) {
	//		cout<<Matrix[i][j]<<" ";
	//	}
	//	cout<<endl;
	//}

	// 高斯消元法	
	const size_t	right_bit = b_max_size;
	const size_t	flag_bit = right_bit + 1;	
	for (idx = 0; idx < right_bit; idx++) {	// b_max_size 轮
		for (size_t i = 0; i < right_bit; i++) {	// 寻找 (x^idx) 存在的行
			if (!Matrix[i][idx] || Matrix[i][flag_bit])	// 跳过 (x^idx) 不存在，或已处理的行
				continue;				
			Matrix[i][flag_bit] = 1;	// 设置已处理标志
			for (size_t j = 0; j < right_bit; j++) {
				if (!Matrix[j][idx] || i == j)
					continue;				
				// 消元
				for (size_t k = idx; k < flag_bit; k++) {
					//if (!Matrix[i][k])
					//	continue;
					Matrix[j][k] += Matrix[i][k];
					Matrix[j][k] %= 2;
				}

				//cout<<"Matrix:"<<endl;
				//for (size_t i = 0; i < b_max_size; i++) {
				//	cout<<"\t";
				//	for (size_t j = 0; j < b_max_size + 1; j++) {
				//		cout<<Matrix[i][j]<<" ";
				//	}
				//	cout<<endl;
				//}

			}		
		}
	}
	// 矩阵求解
	polyInF2				b;
	b.coef.resize(b_max_size, 0);
	for (idx = 0; idx < right_bit; idx++) {
		size_t i = 0;
		for (; i < right_bit; i++) {
			if (Matrix[idx][i]) break;
		}
		if (i == right_bit) {
			if (Matrix[idx][right_bit]) {
				b.coef.clear();
				cerr<<"No solution!"<<endl;
				break;
			} else {
				cerr<<"Warning: more than one solution!"<<endl;
			}			
		}
		b.coef[i] = Matrix[idx][right_bit];
	}
	b.refresh();

	return b;
}

void polyInF2::listPrimitive(const int m) {			

	// 生成次数不高于 m 的不可约多项式表				
	polyInF2::generateIrreducible(m);
	size_t		i = 0;
	//DWORD		Tick = GetTickCount();	
	while (i < polyInF2::irreducible.size() && polyInF2::irreducible[i].coef.size() <= m + 1) {
		if (polyInF2::irreducible[i].coef.size() == m + 1) {
			if (polyInF2::irreducible[i].isPrimitive_step2()) {
				cout<<polyInF2::irreducible[i]<<endl;
			}
		}
		i++;
	}
	//cerr<<"Debug: "<<GetTickCount() - Tick<<" ms elapsed!"<<endl;
}

void polyInF2::generateIrreducible(const int m) {
				
	polyInF2				f;	
	if (polyInF2::irreducible.empty()) {		
		polyInF2::irreducible.push_back(polyInF2("10"));
		polyInF2::irreducible.push_back(polyInF2("11"));
	}		
	// 跳过已计算好的不可约多项式
	f.coef.resize(polyInF2::irreducible[polyInF2::irreducible.size() - 1].coef.size(), 0);	
	f.coef.reserve(m + 1);
	f.coef.push_back(1);
	while (f.coef.size() <= (m + 1)) {		
		size_t				i;
		const size_t		size = f.coef.size();
		const size_t		max_exp = (size - 1) / 2;
		const size_t		list_size = polyInF2::irreducible.size();
		// 整除测试，被除数为不高于 max_exp 次的所有不可约多项式
		i = 0;
		while (i < list_size && polyInF2::irreducible[i].coef.size() <= max_exp + 1) {
			if ((f % polyInF2::irreducible[i]).isZero())
				break;
			i++;
		}
		if (i == list_size || polyInF2::irreducible[i].coef.size() > max_exp + 1) {
			// 都不能整除则为不可约多项式
			//cerr<<"Debug: irreducible "<<f<<endl;
			polyInF2::irreducible.push_back(f);
		}
		// 生成下一个待测多项式
		i = 0;
		while (i < size) {
			if (f.coef[i] == 0) {
				f.coef[i] = 1;
				break;
			} else {
				f.coef[i] = 0;
			}
			i++;
		}
		if (i == size) {
			f.coef.push_back(1);
		}
	}
	//for (size_t i = 0; i < polyInF2::irreducible.size(); i++) {	
	//	cerr<<"Debug: irreducible "<<polyInF2::irreducible[i]<<endl;
	//}
}

ostream & operator << (ostream & out, const polyInF2 & rval) {

	polyInF2	f = rval;

	if (f.coef.size() == 1 && f.coef[0] == 0) {
		out<<"0";
	} else {
		int			idx = (int)f.coef.size() - 1;
		bool		isFirst = true;

		while (idx >= 0) {
			if (f.coef[idx] == 1) {
				if (isFirst){
					isFirst = false;
				} else {
					out<<"+";
				}
				if (idx == 0) {
					out<<"1";
				} else if (idx == 1) {
					out<<"x";
				} else {
					out<<"x^"<<idx;
				}
			}
			idx--;
		}
	}
	return out;
}

istream & operator >> (istream & in, polyInF2 & rval) {

	string		str;

	in>>str;
	rval = polyInF2(str);
	return in;
}

string polyInF2::ToStr(const polyInF2 &f)
{
	string str;
	for(int i=f.coef.size()-1;i>=0;i--)
	{
		char sz[10]={0};
                sprintf(sz,"%d",f.coef[i]);
                str+=sz;
		//str+=itoa(f.coef[i],sz,10);
	}
	return str;
}

unsigned long long polyInF2::ToUINT64(const polyInF2 &f)
{
	unsigned long long lret=0;
	for(int i=0;i<f.coef.size();i++)
	{
		if(f.coef[i]==1)
			lret+=(unsigned long long)(pow((double)2,i)+0.5);
	}
	return lret;
}
