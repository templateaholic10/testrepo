/*! @file
    @brief 演算子の自動実装のためのCRTP基底クラス
    @auther yanteyon10
    @date 12/30
*/

#ifndef AUTOOPERATION_HPP
#define AUTOOPERATION_HPP

namespace devel {
    /*! @brief 加減乗除
    */
    template <class Derived, class Rhs = Derived>
    class Four_operations {
    public:
        Derived operator+(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval += rhs;
            return retval;
        }
        Derived operator-(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval -= rhs;
            return retval;
        }
        Derived operator*(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval *= rhs;
            return retval;
        }
        Derived operator/(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval /= rhs;
            return retval;
        }
        Derived operator%(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval %= rhs;
            return retval;
        }
    };

    /*! @brief 加減乗除余
    */
    template <class Derived, class Rhs = Derived>
    class Five_operations {
    public:
        Derived operator+(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval += rhs;
            return retval;
        }
        Derived operator-(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval -= rhs;
            return retval;
        }
        Derived operator*(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval *= rhs;
            return retval;
        }
        Derived operator/(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval /= rhs;
            return retval;
        }
        Derived operator%(const Rhs &rhs) const
        {
            Derived retval(static_cast<const Derived &>(*this));
            retval %= rhs;
            return retval;
        }
    };

    /*! @brief 順序
    */
    template <class Derived>
    class Order_operations1 {
    public:
        bool operator>=(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return rhs <= dthis;
        }
        bool operator==(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return dthis <= rhs && dthis >= rhs;
        }
        bool operator!=(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return !(dthis == rhs);
        }
        bool operator<(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return dthis <= rhs && dthis != rhs;
        }
        bool operator>(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return dthis >= rhs && dthis != rhs;
        }
    };

    /*! @brief 順序
    */
    template <class Derived>
    class Order_operations2 {
    public:
        bool operator>(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return rhs < dthis;
        }
        bool operator!=(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return dthis < rhs || dthis > rhs;
        }
        bool operator==(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return !(dthis != rhs);
        }
        bool operator<=(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return (dthis < rhs) || (dthis == rhs);
        }
        bool operator>=(const Derived &rhs) const
        {
            const Derived &dthis = static_cast <const Derived &>(*this);
            return (dthis > rhs) || (dthis == rhs);
        }
    };
};

#endif
