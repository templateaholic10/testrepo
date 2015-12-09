/*! @file
    @brief 反復法の基底クラス
    @author yanteyon10
    @date 11/26
*/
#ifndef ITERATIVE_HPP
#define ITERATIVE_HPP

#include <iostream>
#include <array>

/*! @macro
    @brief 標準出力に表示するステップ周期
*/
#ifndef BREAK_ITERATION
    #define BREAK_ITERATION 1
#endif

/*! @macro
    @brief デフォルト最大反復回数
*/
#ifndef DEFAULT_MAX_REP
    #define DEFAULT_MAX_REP 100000
#endif

namespace optimization {
    /*! @class
        @brief 反復法のCRTP基底クラス．派生クラスにおけるforward，step_cond関数を混入する．主な使い方は
        コンストラクタまたはinitで初期化
        processでシミュレート
        flushで出力

        派生クラスは以下の要件を満たす
        (1) 明示的なコンストラクタを持ち，初期化子リストでIterativeコンストラクタをコールする
        (2) void forward()関数，bool stop_cond() const関数の実装を持つ．void record(const int k) const関数を必要に応じてオーバーライドする
        (3) IterativeクラスメンバにアクセスするにはthisをIterative*またはconst Iterative*にstatic_castする．コンストラクタでキャストして保持しておくとよい
        @tparam Derived 派生クラス
        @tparam length バッファの長さ
        @tparam Args 反復対象の型たち
    */
    template <class Derived, int length, typename ... Args>
    class Iterative;

    template <class Derived, int length, typename elem_type>
    class Iterative <Derived, length, elem_type> {
    private:
        // derived_pはthis自身なのでnew-deleteとは関係ない
        Derived *derived_p;
    public:
        using type = Iterative <Derived, length, elem_type>;
    public:
        std::array <elem_type, length> x;
        int                            step;
        int                            cum_step;
        const int                      order;
        std::ostream                  &os;
        const char                     delim;
        const int                      max_rep;
    public:
        Iterative() = delete;
        /*! @brief Iterativeクラスのコンストラクタ
            @param order_ 漸化式の階数
            @param os_ 出力ストリーム
            @param delim_ デリミタ
            @param max_rep_ 最大反復数
        */
        Iterative(const int order_, std::ostream &os_, const int delim_=',', const int max_rep_=DEFAULT_MAX_REP)
            : derived_p(static_cast <Derived *>(this)), step(order_), cum_step(order_), order(order_), os(os_), delim(delim_), max_rep(max_rep_)
        {
        }

        Iterative(const type&)              = delete;
        Iterative(type&&)                   = delete;
        type        &operator=(const type&) = delete;
        type        &operator=(type&&)      = delete;
        virtual void forward()              = 0;
        void         step_in();
        virtual void record(const int k) const;
        void         flush(const int start=0);
        void         flush(const int start, const int last);
        void         next();
        virtual bool stop_cond() const = 0;
        void         process();
        void         process(const int to);
    };

    /*! @brief ステップインする関数
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::step_in()
    {
        if (cum_step % BREAK_ITERATION == 0) {
            std::cout << "STEP: " << cum_step << std::endl;
        }
        assert(step < length);
        derived_p->forward();
        step++;
        cum_step++;
    }

    /*! @brief 1ステップごとの出力関数
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::record(const int i) const
    {
        os << x[i] << std::endl;
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::flush(int start)
    {
        for (int i = start; i < step; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::flush(int start, int last)
    {
        for (int i = start; i < step && i < last; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief 溜まったバッファを吐き出して折り返す関数
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::next()
    {
        flush(0, length - order);
        for (int i = 0; i < order; i++) {
            x[i] = x[length - order + i];
        }
        step = order;
    }

    /*! @brief プロセッサ
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::process()
    {
        while (cum_step < max_rep) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    /*! @brief プロセッサ
        @param to ここまでステップを進める
    */
    template <class Derived, int length, typename elem_type>
    void Iterative <Derived, length, elem_type>::process(int to)
    {
        while (cum_step < to) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    template <class Derived, int length, typename elem_type1, typename elem_type2>
    class Iterative <Derived, length, elem_type1, elem_type2> {
    private:
        // derived_pはthis自身なのでnew-deleteとは関係ない
        Derived *derived_p;
    public:
        using type = Iterative <Derived, length, elem_type1, elem_type2>;
    public:
        std::array <elem_type1, length> x1;
        std::array <elem_type2, length> x2;
        int                             step;
        int                             cum_step;
        const int                       order;
        std::ostream                   &os1;
        std::ostream                   &os2;
        const char                      delim;
        const int                       max_rep;
    public:
        Iterative() = delete;
        /*! @brief Iterativeクラスのコンストラクタ
            @param order_ 漸化式の階数
            @param os1_ 出力ストリーム
            @param os2_ 出力ストリーム
            @param delim_ デリミタ
            @param max_rep_ 最大反復数
        */
        Iterative(const int order_, std::ostream &os1_, std::ostream &os2_, const int delim_=',', const int max_rep_=DEFAULT_MAX_REP)
            : derived_p(static_cast <Derived *>(this)), step(order_), cum_step(order_), order(order_), os1(os1_), os2(os2_), delim(delim_), max_rep(max_rep_)
        {
        }

        Iterative(const type&)              = delete;
        Iterative(type&&)                   = delete;
        type        &operator=(const type&) = delete;
        type        &operator=(type&&)      = delete;
        virtual void forward()              = 0;
        void         step_in();
        virtual void record(const int k) const;
        void         flush(const int start=0);
        void         flush(const int start, const int last);
        void         next();
        virtual bool stop_cond() const = 0;
        void         process();
        void         process(const int to);
    };

    /*! @brief ステップインする関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::step_in()
    {
        if (cum_step % BREAK_ITERATION == 0) {
            std::cout << "STEP: " << cum_step << std::endl;
        }
        assert(step < length);
        derived_p->forward();
        step++;
        cum_step++;
    }

    /*! @brief 1ステップごとの出力関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::record(const int i) const
    {
        os1 << x1[i] << std::endl;
        os2 << x2[i] << std::endl;
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::flush(int start)
    {
        for (int i = start; i < step; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::flush(int start, int last)
    {
        for (int i = start; i < step && i < last; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief 溜まったバッファを吐き出して折り返す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::next()
    {
        flush(0, length - order);
        for (int i = 0; i < order; i++) {
            x1[i] = x1[length - order + i];
            x2[i] = x2[length - order + i];
        }
        step = order;
    }

    /*! @brief プロセッサ
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::process()
    {
        while (cum_step < max_rep) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    /*! @brief プロセッサ
        @param to ここまでステップを進める
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2>
    void Iterative <Derived, length, elem_type1, elem_type2>::process(int to)
    {
        while (cum_step < to) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    class Iterative <Derived, length, elem_type1, elem_type2, elem_type3> {
    private:
        // derived_pはthis自身なのでnew-deleteとは関係ない
        Derived *derived_p;
    public:
        using type = Iterative <Derived, length, elem_type1, elem_type2, elem_type3>;
    public:
        std::array <elem_type1, length> x1;
        std::array <elem_type2, length> x2;
        std::array <elem_type3, length> x3;
        int                             step;
        int                             cum_step;
        const int                       order;
        std::ostream                   &os1;
        std::ostream                   &os2;
        std::ostream                   &os3;
        const char                      delim;
        const int                       max_rep;
    public:
        Iterative() = delete;
        /*! @brief Iterativeクラスのコンストラクタ
            @param order_ 漸化式の階数
            @param os1_ 出力ストリーム
            @param os2_ 出力ストリーム
            @param os3_ 出力ストリーム
            @param delim_ デリミタ
            @param max_rep_ 最大反復数
        */
        Iterative(const int order_, std::ostream &os1_, std::ostream &os2_, std::ostream &os3_, const int delim_=',', const int max_rep_=DEFAULT_MAX_REP)
            : derived_p(static_cast <Derived *>(this)), step(order_), cum_step(order_), order(order_), os1(os1_), os2(os2_), os3(os3_), delim(delim_), max_rep(max_rep_)
        {
        }

        Iterative(const type&)              = delete;
        Iterative(type&&)                   = delete;
        type        &operator=(const type&) = delete;
        type        &operator=(type&&)      = delete;
        virtual void forward()              = 0;
        void         step_in();
        virtual void record(const int k) const;
        void         flush(const int start=0);
        void         flush(const int start, const int last);
        void         next();
        virtual bool stop_cond() const = 0;
        void         process();
        void         process(const int to);
    };

    /*! @brief ステップインする関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::step_in()
    {
        if (cum_step % BREAK_ITERATION == 0) {
            std::cout << "STEP: " << cum_step << std::endl;
        }
        assert(step < length);
        derived_p->forward();
        step++;
        cum_step++;
    }

    /*! @brief 1ステップごとの出力関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::record(const int i) const
    {
        os1 << x1[i] << std::endl;
        os2 << x2[i] << std::endl;
        os3 << x3[i] << std::endl;
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::flush(int start)
    {
        for (int i = start; i < step; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::flush(int start, int last)
    {
        for (int i = start; i < step && i < last; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief 溜まったバッファを吐き出して折り返す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::next()
    {
        flush(0, length - order);
        for (int i = 0; i < order; i++) {
            x1[i] = x1[length - order + i];
            x2[i] = x2[length - order + i];
            x3[i] = x3[length - order + i];
        }
        step = order;
    }

    /*! @brief プロセッサ
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::process()
    {
        while (cum_step < max_rep) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    /*! @brief プロセッサ
        @param to ここまでステップを進める
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3>::process(int to)
    {
        while (cum_step < to) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    class Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4> {
    private:
        // derived_pはthis自身なのでnew-deleteとは関係ない
        Derived *derived_p;
    public:
        using type = Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>;
    public:
        std::array <elem_type1, length> x1;
        std::array <elem_type2, length> x2;
        std::array <elem_type3, length> x3;
        std::array <elem_type4, length> x4;
        int                             step;
        int                             cum_step;
        const int                       order;
        std::ostream                   &os1;
        std::ostream                   &os2;
        std::ostream                   &os3;
        std::ostream                   &os4;
        const char                      delim;
        const int                       max_rep;
    public:
        Iterative() = delete;
        /*! @brief Iterativeクラスのコンストラクタ
            @param order_ 漸化式の階数
            @param os1_ 出力ストリーム
            @param os2_ 出力ストリーム
            @param os3_ 出力ストリーム
            @param os4_ 出力ストリーム
            @param delim_ デリミタ
            @param max_rep_ 最大反復数
        */
        Iterative(const int order_, std::ostream &os1_, std::ostream &os2_, std::ostream &os3_, std::ostream &os4_, const int delim_=',', const int max_rep_=DEFAULT_MAX_REP)
            : derived_p(static_cast <Derived *>(this)), step(order_), cum_step(order_), order(order_), os1(os1_), os2(os2_), os3(os3_), os4(os4_), delim(delim_), max_rep(max_rep_)
        {
        }

        Iterative(const type&)              = delete;
        Iterative(type&&)                   = delete;
        type        &operator=(const type&) = delete;
        type        &operator=(type&&)      = delete;
        virtual void forward()              = 0;
        void         step_in();
        virtual void record(const int k) const;
        void         flush(const int start=0);
        void         flush(const int start, const int last);
        void         next();
        virtual bool stop_cond() const = 0;
        void         process();
        void         process(const int to);
    };

    /*! @brief ステップインする関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::step_in()
    {
        if (cum_step % BREAK_ITERATION == 0) {
            std::cout << "STEP: " << cum_step << std::endl;
        }
        assert(step < length);
        derived_p->forward();
        step++;
        cum_step++;
    }

    /*! @brief 1ステップごとの出力関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::record(const int i) const
    {
        os1 << x1[i] << std::endl;
        os2 << x2[i] << std::endl;
        os3 << x3[i] << std::endl;
        os4 << x4[i] << std::endl;
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::flush(int start)
    {
        for (int i = start; i < step; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief バッファをはき出す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::flush(int start, int last)
    {
        for (int i = start; i < step && i < last; i++) {
            derived_p->record(i);
        }
    }

    /*! @brief 溜まったバッファを吐き出して折り返す関数
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::next()
    {
        flush(0, length - order);
        for (int i = 0; i < order; i++) {
            x1[i] = x1[length - order + i];
            x2[i] = x2[length - order + i];
            x3[i] = x3[length - order + i];
            x4[i] = x4[length - order + i];
        }
        step = order;
    }

    /*! @brief プロセッサ
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::process()
    {
        while (cum_step < max_rep) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }

    /*! @brief プロセッサ
        @param to ここまでステップを進める
    */
    template <class Derived, int length, typename elem_type1, typename elem_type2, typename elem_type3, typename elem_type4>
    void Iterative <Derived, length, elem_type1, elem_type2, elem_type3, elem_type4>::process(int to)
    {
        while (cum_step < to) {
            step_in();
            if (derived_p->stop_cond()) {
                return;
            }
            if (step >= length) {
                next();
            }
        }
    }
}

#endif
