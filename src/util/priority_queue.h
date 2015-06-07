//
//  PriorityQueue.h
//  orange
//
//  Created by Kevin on 14-6-10.
//
//

#ifndef orange_PriorityQueue_h
#define orange_PriorityQueue_h

#include<algorithm>
#include<vector>
#include<functional>
#include<iostream>

namespace ora {
    
    template<class T, class compare=std::less<T>, class container=std::vector<T>>
    class PriorityQueue
    {
    public:
        typedef typename container::value_type value_type;
        typedef typename container::size_type size_type;
        typedef typename container::iterator iter_type;
        typedef container container_type;
        
    protected:
        compare compare_;
        container container_;
        
    public:
        explicit PriorityQueue(const compare & com = compare(), const container & cont = container()):
        compare_(com),
        container_(cont)
        {
            std::make_heap(container_.begin(), container_.end(), compare_);
        }
        
        template<class initer>
        PriorityQueue(initer begin, initer last, const compare& com = compare(), const container & cont = container()):
        compare_(com),
        container_(cont)
        {
            container_.insert(container_.end(), begin, last);
            make();
        }
        
        void make()
        {
            std::make_heap(container_.begin(), container_.end(), compare_);
        }
        
        void push(const value_type& x)
        {
            container_.push_back(x);
            std::push_heap(container_.begin(), container_.end(), compare_);
        }
        
        void pop()
        {
            std::pop_heap(container_.begin(), container_.end(), compare_);
            container_.pop_back();
        }
        
        //!!!必须返回引用
        container & getContainer()
        {
            return container_;
        }
        
        const value_type& top() const { return container_.front(); }
        bool empty() const { return container_.empty(); }
        size_type size() const { return container_.size(); }
        iter_type begin() { return container_.begin(); }
        iter_type end() { return container_.end(); }
        
    };
    
}; // end of namespace ora

#endif
