// dpoly.cpp: implementation of the dpoly class.
//
//////////////////////////////////////////////////////////////////////
#include "dpoly.h"


#define EPSILON 1.0e-10	// zero double

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
dpoly::dpoly()
{
	m_dpoly.clear();
}

dpoly::dpoly(string file)   
{
	ReadFromFile(file);
}

dpoly::dpoly(const vector<double>& cof, const vector<double>& deg)
{
	m_dpoly.clear();
	int n= cof.size();
	for (int i=0;i<n;i++)
	{
		dpolynode nd;
		nd.cof=cof[i]; 
		nd.deg=deg[i];
		AddOneTerm(nd);
	}
}

dpoly::dpoly(const vector<double>& cof)
{
	m_dpoly.clear();
	int n= cof.size();
	for (int i=0;i<n;i++)
	{
		dpolynode nd;
		nd.cof=cof[i]; 
		nd.deg=i;
		AddOneTerm(nd);
	}
}

dpoly::dpoly(double *cof,double *deg,int n)
{
	m_dpoly.clear();
	for (int i=0;i<n;i++)
	{
		dpolynode nd;
		nd.cof=cof[i]; 
		nd.deg=deg[i];
		AddOneTerm(nd);
	}
}

dpoly::~dpoly()
{
	m_dpoly.clear();
}

//overload operator + function
dpoly& dpoly::operator+(dpoly &right)
{
	dpoly *tmpPoly = new dpoly();
	tmpPoly->m_dpoly=this->m_dpoly;
	list<dpolynode> tp;
	tp = right.m_dpoly;
	
	list<dpolynode>::iterator it;
	for (it=tp.begin();it!=tp.end();it++)
	{
		tmpPoly->AddOneTerm(*it);
	}
	
	return *tmpPoly;
}

//overload operator - function
dpoly& dpoly::operator-(dpoly &right)
{
	dpoly *tmpPoly = new dpoly();
	tmpPoly->m_dpoly=this->m_dpoly;
	list<dpolynode> tp;
	tp = right.m_dpoly;
	
	list<dpolynode>::iterator it;
	for (it=tp.begin();it!=tp.end();it++)
	{
		(*it).cof=-(*it).cof;
		tmpPoly->AddOneTerm(*it);
	}
	
	return *tmpPoly;
}

//overload operator * function
dpoly& dpoly::operator*(dpoly &right)
{
	dpoly *tmpPoly = new dpoly();
	
    list<dpolynode> p1,p2;
	p1=this->m_dpoly;
	p2=right.m_dpoly;


    list<dpolynode>::iterator it1,it2;
    for (it1=p1.begin();it1!=p1.end();it1++)
		for (it2=p2.begin();it2!=p2.end();it2++)
		{
			dpolynode tn1,tn2;
			tn1 = *it1; 
			tn2=*it2;
			tn1.cof*=tn2.cof;
			tn1.deg+=tn2.deg;
			tmpPoly->AddOneTerm(tn1);
		}
	
	return *tmpPoly;
}

//overload operator = function
dpoly& dpoly::operator=(dpoly &right)
{
	this->m_dpoly = right.m_dpoly;
	return *this;
}

void dpoly::ReadFromFile(string file)
{
	m_dpoly.clear();
	ifstream inp;
    inp.open(file.c_str());
	char ch;
	int n;
	inp>>ch; 
	inp>>n;
	for (int i=0;i<n;i++)
	{
		dpolynode nd;
		inp>>nd.deg; 
		inp>>nd.cof;

        AddOneTerm(nd);
	}
	inp.close();
}

void dpoly::AddOneTerm(dpolynode term)  // insert one term into the list 
{
	int n=m_dpoly.size();
	if(n==0) { m_dpoly.push_back(term); return;}

	if(fabs(term.cof)<EPSILON) 
	{
		return; // if the coefficient is zero
	}

	if(term.deg<0) 
	{ 
		cout<<"Degree can not be negative!"<<endl; 
		return; 
	}
    
	list<dpolynode>::iterator it;  // iterator of list
	dpolynode td;
	for (it=m_dpoly.begin();it!=m_dpoly.end();it++)
	{
		td = *it;
		if(term.deg<td.deg) 
		{
			break;
		}
	}
	
	// add 
	it--;
	td=*it;

	if (term.deg==td.deg)
	{
		(*it).cof+=term.cof;
		if (fabs((*it).cof)<EPSILON)
		{
			m_dpoly.erase(it);
		}
		return;
	}
    it++;
    
	// insert
    if (it==m_dpoly.end())
    {
		m_dpoly.push_back(term);
		return;
    }

	m_dpoly.insert(it,term);
}

void dpoly::Print()    // print the polynomial
{
	list<dpolynode>::iterator it;
	cout<<"The polynomial is:"<<endl;

	for (it=m_dpoly.begin();it!=m_dpoly.end();it++)
	{
		dpolynode td = *it;
		
        if (it!=m_dpoly.begin())
        {
			if (td.cof>0)
			{
				cout<<"+";
			}
		}
        cout<<td.cof<<" ";
        
		if (td.deg!=0)
        {
			cout<<"x^"<<td.deg;
        }
		cout<<" ";
	}

	cout<<endl;
}