#include "ObjectPool.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

int alive = 0;

class A
{
public:
    A()
    {
        alive++;
    }
    ~A()
    {
        alive--;
    }
};

template<>
struct PoolTraits<A>
{
    static const PoolType kType = PoolType::Disabled;
};

class B
{
public:
    B()
    {
        //cout << ++alive << endl;
    }
    ~B()
    {
        //cout << --alive << endl;
    }
};

template<>
struct PoolTraits<B>
{
    static const PoolType kType = PoolType::ThreadLocal;
    static const int kLowWaterMark = 16;
    static const int kHighWaterMark = 128;
    static const long long kMaxIdle = 1000;  // ms
};

class C
{
public:
    unsigned long long x;
    unsigned long long y;
    unsigned long long z;
    C()
    {
        //cout << ++alive << endl;
    }
    ~C()
    {
        //cout << --alive << endl;
    }
};

template<>
struct PoolTraits<C>
{
    //static const PoolType kType = PoolType::Disabled;
    static const PoolType kType = PoolType::ThreadLocal;
    static const int kLowWaterMark = 16;
    static const int kHighWaterMark = 128;
    static const long long kMaxIdle = 3000;  // ms
};

typedef struct tag_train{
    unsigned int groupId;
    unsigned int issi1_state;     //��̨1���ػ�״̬
    unsigned int issi2_state;     //��̨2���ػ�״̬
    unsigned int gssi;		//���
    unsigned int cache_gssi;			//��ʱת�鱣����gssi		2020/11/2
    //wchar_t groupLabel[MAX_TRAIN_GROUPLABEL_LEN]; 
    unsigned int relate_headrcid;//������ͷ��̨��Դ
    unsigned int relate_tailrcid;//������β��̨��Դ
    std::vector<int> relate_otherRcid;//���������������ϵĵ�̨��Դ
    std::map<int, int> section_gssiMap;

    int pathId;        //��·��
    std::string  trackName;      //������� 10�ֽ�,��11�ֽ����ַ�����������
    std::string  map_trackName;      //
    int coachNumber;	//���κ�
    int driverId;		//˾����
    byte sectionId;		//���Σ����ߡ������Ρ�ͣ����
    byte direction;		//�������С�����
    int position;		//������ֽ�Ϊ0�����ʾ��վ��(1~33)��
    //������ֽڷ�0�����г�λ�ڸ��ֽڴ���ĳ�վ�͵��ֽڴ���ĳ�վ�м�
    std::string position_name;
    std::string station_area_name;	//���ڵ�վ����
    std::string did;         //Ŀ�ĵ�ID
    int destStation; //�յ�վ��
    std::string destStation_name;
    byte atsStatus; //ATS״̬ 0�ر� 1��
    byte broadcastStatus;
    byte headNum;  //0:�޳�ͷ 1:1�ų�̨Ϊ��ͷ 2:2�ų�̨Ϊ��ͷ
    //byte radio_status; //0:���رգ�1����̨1������2����̨2������3��������
    int pre_station;
    std::string pre_station_name;
    int  next_station;
    std::string next_station_name;
    std::string driveMode;
    std::string speedProfile;
    byte fastSlowFlag;
    unsigned char assignMode; //���ü��أ���̬����
}RDS_TRAIN;

template<>
struct PoolTraits<RDS_TRAIN>
{
    //static const PoolType kType = PoolType::Disabled;
    static const PoolType kType = PoolType::ThreadLocal;
    static const int kLowWaterMark = 16;
    static const int kHighWaterMark = 128;
    static const long long kMaxIdle = 0;  // ms
};

void testWashOut()
{
    std::vector<PooledPtr<C>> ptrs;
    for (int i = 0; i < 1000; i++)
    {
        ptrs.push_back(object_pool::Get<C>());
        // cout << ptrs[i].Get() << endl;
    }
    ptrs.clear();
    for (int i = 0; i != 200; ++i)
    {
        Sleep(10);
        object_pool::Get<C>();  // Trigger wash out if possible.
    }
    cout << alive << endl;
}

void diff()
{
    for (int i = 0; i < 1000; i++)
    {
        C* c = (C*)object_pool::Get<C>().Leak();
        B* b = (B*)object_pool::Get<B>().Leak();
        delete c;
        delete b;
    }
}

void benchmark()
{
    const int times = 3000000;
    unsigned long long start, end;
    start = object_pool::tls::GetCurrentTimestamp();
    for (int i = 0; i < times; i++)
    {
        RDS_TRAIN* c = new RDS_TRAIN();
        delete c;
    }
    end = object_pool::tls::GetCurrentTimestamp();
    cout << end - start <<endl;
    std::vector<PooledPtr<RDS_TRAIN>> ptrs;
    for (int i = 0; i < times; i++)
    {
        ptrs.push_back(object_pool::Get<RDS_TRAIN>());
        // cout << ptrs[i].Get() << endl;
    }
    ptrs.clear();
    start = object_pool::tls::GetCurrentTimestamp();
    for (int i = 0; i < times; i++)
    {
        object_pool::Get<RDS_TRAIN>();
    }
    end = object_pool::tls::GetCurrentTimestamp();
    cout << end - start <<endl;
}

int main()
{
    //testWashOut();
    //diff();
    benchmark();
    return 0;
}