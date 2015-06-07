#ifndef H_LIST_PROXY_H
#define H_LIST_PROXY_H


#include <list>

namespace ora
{
    enum class list_proxy_state
    {
        add_front = 1,
        add_back,
        remove,
        clear,
    };


    template<typename T>
    struct list_proxy_node
    {
        typedef T value_type;

        list_proxy_node(const value_type & v, list_proxy_state state)
            : value_(v)
            , state_(state)
        {
        }

        value_type  value_;
        list_proxy_state  state_;
    };


    template<typename T>
    class list_proxy
    {
    public:
        typedef T value_type;
        typedef std::list<value_type> container;
        typedef list_proxy_node<value_type> node_type;

        typedef typename container::iterator            iterator;
        typedef typename container::const_iterator      const_iterator;
        typedef typename container::reverse_iterator    reverse_iterator;
        typedef typename container::const_reverse_iterator const_reverse_iterator;

        list_proxy()
            : lockindex_(0)
        {}

        value_type & back()
        {
            return list_.back();
        }
        const value_type & back() const
        {
            return list_.back();
        }

        value_type & front()
        {
            return list_.front();
        }
        const value_type & front() const
        {
            return list_.front();
        }

        iterator begin()
        {
            return list_.begin();
        }
        iterator end()
        {
            return list_.end();
        }

        const_iterator begin() const
        {
            return list_.begin();
        }
        const_iterator end() const
        {
            return list_.end();
        }

        reverse_iterator rbegin()
        {
            return list_.rbegin();
        }
        reverse_iterator rend()
        {
            return list_.rend();
        }

        const_reverse_iterator rbegin()const
        {
            return list_.rbegin();
        }
        const_reverse_iterator rend()const
        {
            return list_.rend();
        }


        bool if_locked() const
        {
            return lockindex_ != 0;
        }
        void lock()
        {
            ++lockindex_;
        }
        void unlock()
        {
            --lockindex_;
            if(!if_locked()) merge_cache();
        }

        void refresh()
        {
            if(!if_locked()) merge_cache();
        }


        void push_back(const value_type & v)
        {
            if(if_locked()) cache_.push_back(node_type(v, list_proxy_state::add_back));
            else list_.push_back(v);
        }

        void push_front(const value_type & v)
        {
            if(if_locked()) cache_.push_back(node_type(v, list_proxy_state::add_front));
            else list_.push_front(v);
        }

        void remove(const value_type & v)
        {
            if(if_locked()) cache_.push_back(node_type(v, list_proxy_state::remove));
            else list_.remove(v);
        }

        void clear()
        {
            if(if_locked()) cache_.push_front(node_type(value_type(), list_proxy_state::clear));
            else
            {
                list_.clear();
                cache_.clear();
            }
        }

        //以下几方法是不安全的，调用者需要判断list是否处于锁定状态

        void erase(iterator it)
        {
            assert(if_locked() && "the list is locked!");

            list_.erase(it);
        }

        iterator find(const value_type & v) const
        {
            return list_.find(v);
        }

        template<typename Pred>
        iterator find_if(const value_type & v, Pred pred) const
        {
            return list_.find_if(v, pred);
        }


    protected:

        void merge_cache()
        {
            for(const node_type & v : cache_)
            {
                if(v.state_ == list_proxy_state::clear)
                {
                    list_.clear();
                    break;
                }
                else if(v.state_ == list_proxy_state::add_front)
                {
                    list_.push_front(v.value_);
                }
                else if(v.state_ == list_proxy_state::add_back)
                {
                    list_.push_back(v.value_);
                }
                else if(v.state_ == list_proxy_state::remove)
                {
                    list_.remove(v.value_);
                }
            }

            cache_.clear();
        }


        int         lockindex_;
        container   list_;
        std::list<list_proxy_node<value_type>> cache_;
    };

}// end namespace ora

#endif //H_LIST_PROXY_H
