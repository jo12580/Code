#include<iostream>
using namespace std;
//���������
class Cshape
{public:

    virtual void DispAttr()=0;
    virtual double Sarea()=0;
    virtual double Volume()=0;
    double r;
    double v;
    double s;
    double h;
    double l;
    double w;
};

//���峤������
class Cuboid :public Cshape
{public:
    
    Cuboid ()
    {
        cout << "This is one information about  a cuboid ."<< endl;
        
    }
    virtual void DispAttr()
    {
        
        
        cout << " length:" << l << endl;
        cout << " width:" << w << endl;
        cout << "height:" << h << endl;
        cout << " Surface area:" << Sarea() << endl;
        cout << "Volume:" << Volume() << endl;
    
    }
    virtual double Sarea()
    {
        s = l*(w+h)*2+w*h*2;
        return s;
    }
    virtual double Volume()
    {
        v=l*w*h ;
        return v;
    }

    

};
//��������
class Globe:public Cshape
{public:
    
    Globe()
    {
        cout << "This is an information about a globe."<< endl;
        
    }
    virtual void DispAttr()
    {
        
        
        cout << "radius:" << r << endl;
        cout << "Surface area:" << Sarea() << endl;
        cout << "Volume:" << Volume() << endl;
    
    }
    virtual double Sarea()
    {
        s = 3.14*r*r;
        return s;
    }
    virtual double Volume()
    {
        v= (4.0/3)*3.14*r*r*r;
        return v;
    }
};
//����������
class Cube:public Cshape
{public:
    
    Cube()
    {
        cout << "This is an information about a cube."<< endl;
        
    }
    virtual void DispAttr()
    {
        
        
        cout << "Edge Length :" << r << endl;
        cout << "Surface area:" << Sarea() << endl;
        cout << "Volume:" << Volume() << endl;
    
    }
    virtual double Sarea()
    {
        s = r*r*6;
        return s;
    }
    virtual double Volume()
    {
        v= r*r*r;
        return v;
    }
    

};
//����Բ����
class Cylinder :public Cshape
{public:
    
    Cylinder ()
    {
        cout << "This is one information about  a cylinder ."<< endl;
        
    }
    virtual void DispAttr()
    {
        
        
        cout << " radius:" << r << endl;
        cout << "height:" << h << endl;
        cout << " Surface area:" << Sarea() << endl;
        cout << "Volume:" << Volume() << endl;
    
    }
    virtual double Sarea()
    {
        s = 2*3.14*r*h+2*3.14*r*r;
        return s;
    }
    virtual double Volume()
    {
        v=3.14*r*r*h;
        return v;
    }

    

};

void test01()
{
    //����
    Cshape * g1 = new Globe;
    g1->r=3;
    g1->DispAttr();
    delete g1;
    cout << "------------" << endl;
    //������
    g1 = new Cuboid ;
    g1->l=4;
    g1->w=3;
    g1->h=5;
    g1->DispAttr();
    delete g1;
    cout << "------------" << endl;
    //������
    g1 = new Cube ;
    g1->r=4;
    g1->DispAttr();
    delete g1;
    cout << "------------" << endl;
    //Բ����
    g1 = new Cylinder ;
    g1->r=4;
    g1->h=5;
    g1->DispAttr();
    delete g1;
    
}



int main()
{
    
    test01();
    return 0;
}