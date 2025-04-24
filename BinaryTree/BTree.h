#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <future>
#include <map>
#include <memory>
#include <initializer_list>
#include <atomic>
#include <set>
#include <string>

#define CXX_STANDARD 17

template<typename T> 
using sort_ptr = bool (*)(T, T);

template<typename T> class mcontainer;
template<typename T> void bubble_sort(mcontainer<T>& c, sort_ptr<T> p);

template<typename T>
struct BTree
{
	BTree(T d, int Depth) :data(d),depth(Depth) {}
	BTree* parent = nullptr;
	BTree* left = nullptr;
	BTree* right = nullptr;
	T data;
	int depth;		//深度
};

template<typename T>
struct mdlist
{
	mdlist() :back(nullptr), next(nullptr) { std::cout << "mdlist()\n"; }
	mdlist(T d) :back(nullptr), next(nullptr), data(d) { std::cout << "mdlist(int d)\n"; }
	mdlist(const mdlist& o) = delete;
	mdlist& operator=(const mdlist& o) = delete;
	mdlist(mdlist&& o) noexcept :back(o.back),next(o.next),data(o.data) {
		o.back = nullptr;
		o.next = nullptr;
		o.data = 0;
		std::cout << "mdlist(mdlist&& o)\n";
	}
	mdlist& operator=(mdlist&& o)noexcept {
		back = o.back;
		next = o.next;
		data = o.data;

		o.back = nullptr;
		o.next = nullptr;
		o.data = 0;
		return *this;
		std::cout << "operator=(mdlist&& o)\n";
	}
	~mdlist()
	{
		std::cout << "~mdlist\n";
		back = nullptr;
		next = nullptr;
	}
	mdlist* back;
	mdlist* next;
	T data;
};

template<typename T>
mdlist<T>* createMdlistNode(T data);
template<typename T>
mdlist<T>* createMdlistNode();
/*
void destoryMdlistHead(mdlist* h);
void headInsert(mdlist** h, int data);
void tailInsert(mdlist** h, int data);
void posInsert(mdlist **h, int idx, int data);		//按照位置插入
void removeOne(mdlist **h, int data);
void removeAt(mdlist** h, int idx);
int  findOne(mdlist** h, int data);
int  mdlist_size(mdlist** h);
void showMdlist(mdlist** h);
void showMdlist_tail(mdlist** t);
void sort_mdlist();*/

template<typename T>
class mcontainer
{
public:
	mcontainer() {
		head = createMdlistNode<T>();
		tail = createMdlistNode<T>();

		head->next = tail;
		tail->back = head;
	}

	mcontainer(T d) {
		head = createMdlistNode<T>(d);
		tail = createMdlistNode<T>(d);

		head->next = tail;
		tail->back = head;
	}
	~mcontainer() {
		mdlist<T>* t;
		while (head)
		{
			t = head;
			head = head->next;
			delete t;
		}
	}

	const mdlist<T>* font()const;
	const mdlist<T>* end()const;
	void push_front(int data);
	void push_back(int data);
	void removeOne(int data);
	void removeAt(int idx);
	void exchange(int first, int second);	//交换两个node的位置
	int size()const;
	bool isEmpty();
	bool findOne(int data);
	mdlist<T>* operator[](int idx);
	T& value(int idx);
	void SortMdlist();

private:
	//std::function<sort_ptr> func;
	mdlist<T>* head;
	mdlist<T>* tail;
};

template<typename T> BTree<T>* createTreeHead(T data, int depth);
template<typename T> void addTreeNode(BTree<T>* head, std::vector<T>& vec, int idx = 0);
template<typename T> void showTreeFront(BTree<T>* head);
template<typename T> bool findNode(BTree<T>* head, T data);
template<typename T> bool TreeRemoveOne(BTree<T>* head, T data);


//thread pool

class threadPool {
public:
	threadPool(int num):stop(false) {
		for (int i = 0; i < num; ++i)
		{
			workers.emplace_back([this]() {
				while (true)
				{
					std::function<void()> func;
					{
						std::unique_lock<std::mutex> lock(m_mutex);
						if (!stop && tasks.empty())
							m_condition.wait(lock);

						if (stop) return;

						func = std::move(tasks.front());
						tasks.pop();
					}
					func();
				}
			});
		}
	}

	void Enqueue(const std::function<void()>& task)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		tasks.emplace(task);
		m_condition.notify_one();
	}

	//异步
	template<typename F, typename... Args>
	auto Enqueue(F&& f, Args&&... arg)->std::future< typename std::result_of<F(Args...)>::type>
	{
#if CXX_STANDARD >= 17
		using resType = typename std::invoke_result_t<F, Args...>;
#else
		using resType = typename std::result_of<F(Args...)>::type;
#endif
		auto task = std::make_shared<std::packaged_task<resType()>>(std::bind(std::forward<F>(f), std::forward<Args>(arg)...));

		std::future<resType> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (stop)
				throw std::runtime_error("enqueue on stopped thread pool");
			tasks.emplace([task]() { (*task)(); });
		}
		m_condition.notify_one();
		return res;
	}

	~threadPool() {
		std::unique_lock<std::mutex> lock(m_mutex);
		stop = true;

		for (auto& work : workers)
		{
			if (work.joinable())
				work.join();
		}
	}
private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::condition_variable m_condition;
	std::mutex m_mutex;
	std::atomic_bool stop;
};


class AB
{
public:
	AB() = default;
	AB(std::initializer_list<int> list) {
		m_data.insert(m_data.end(), list.begin(), list.end());
	}
private:
	std::vector<int> m_data;
};

class xobject
{
public:
	xobject() :xobj_name() {}
	xobject(std::string name) :xobj_name(name) {}
	virtual ~xobject() {}
	const std::string& getName()const { return xobj_name; }
	void setName(const std::string& name) { xobj_name = name; }
protected:
	std::string xobj_name;
};

class buff :public xobject
{
public:	
	buff() : xobject() {}
private:
	int buff_id;
	int buff_type;
	int buff_value;
	int buff_time;
};

class buffsys
{
public:
	static buffsys* getInstance() {
		static buffsys instance;
		return &instance;
	}
	
	void addBuff(buff* b) {
		buffs.push_back(b);
	}
	void removeBuff(buff* b) {
		auto it = std::find(buffs.begin(), buffs.end(), b);
		if (it != buffs.end())
			buffs.erase(it);
	}
private:
	buffsys() {}
	~buffsys()
	{
		for (auto& b : buffs)
		{
			delete b;
		}
	}
	std::vector<buff*> buffs;
};

class Mediator;
class charactor :public xobject
{	
public:
	charactor() :xobject() {
	}
	charactor(const std::string& name) :xobject(name) {
		hp = 0;
		mp = 0;
		atk = 0;
		def = 0;
		speed = 0;
		level = 0;
		exp = 0;
		max_hp = 0;
		max_mp = 0;
		max_exp = 0;
		max_level = 0;
		max_atk = 0;
		max_def = 0;
		max_speed = 0;
	}

	virtual void receive(const std::string& message) = 0;

protected:
	int hp;
	int mp;
	int atk;
	int def;
	int speed;
	int level;
	int exp;
	int max_hp;
	int max_mp;
	int max_exp;
	int max_level;
	int max_atk;
	int max_def;
	int max_speed;
};

class Mediator
{
public:
	virtual ~Mediator() = default;
	virtual void send(const std::string& message, charactor* src,charactor* obj) = 0;

};

class fightMediator :public Mediator
{
public:
	void send(const std::string& message, charactor* src, charactor* obj) override {
		std::cout << "send message:" << message << " to " << obj->getName() << "\n";
		for (auto& c : chactors)
		{
			if (c == obj)
				c->receive(message);
		}
	}
	void addCharactor(charactor* c) {
		chactors.push_back(c);
	}
private:
	std::vector<charactor*> chactors;
};

class player :public charactor
{
public:
	player(Mediator* m) :charactor("player"), m_mediator(m) {
		hp = 100;
		mp = 100;
		atk = 10;
		def = 10;
		speed = 10;
		level = 1;
		exp = 0;
		max_hp = 100;
		max_mp = 100;
		max_exp = 100;
		max_level = 100;
		max_atk = 100;
		max_def = 100;
		max_speed = 100;
	}

    void attack(charactor *dst) {  
		std::cout << "player attack\n";

		std::string atk_str = std::to_string(this->atk);  
		m_mediator->send(atk_str, this,dst);
    }

	void receive(const std::string& message) override {
		std::cout << "player receive message:" << message << "\n";
	}
protected:
	Mediator* m_mediator;
};

class monster :public charactor
{	
public:
	monster() :charactor("monster") {}
	monster(Mediator* m) :charactor(), m_mediator(m) {}
	void receive(const std::string& message) override {
		std::cout << "monster receive message:" << message << "\n";
	}
protected:
	Mediator* m_mediator;
};

