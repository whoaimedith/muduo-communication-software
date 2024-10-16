
#ifndef DATA_STRUCT_DYNARRAY_H
#define DATA_STRUCT_DYNARRAY_H
#include "header.h"
namespace NDataStruct
{
    // 突破固定容量数组限制
    // 容量可以动态增加和减少的数组
    // 适合作为管理动态元素集合的容器
    template<typename T>
    class DynArray
    {
    public:
        DynArray();
        DynArray(int nInitialSize_, const T& nInitialValue_);
        virtual ~DynArray();

        void Add(const T& value_);
        void AddRange(const DynArray& arrItems_);
        void Insert(int nIndex_, const T& value_);
        void DeleteByIndex(int nIndex_ = -1);
        void DeleteByValue(const T& value_);
        void DeleteAll();
        bool SetValue(int nIndex_, const T& value_);
        bool GetValue(int nIndex_, T& value_) const;

        int GetSize() const;
        int GetCapacity() const;
        int Find(std::function<bool(const T&)> fun_) const;

        T& operator[](int nIndex_);
        const T& operator[](int nIndex_) const;
        void Free();
        //T* DeepCopy(int& nSize_);
        DynArray(const DynArray<T>& arrElements_);
        DynArray<T>& operator=(const DynArray<T>& arrElements_);

    private:
        void Check(int nSize_);
        void Shrink();

    private:
        T* m_pSource;// 当T为引用类型时，sizeof无法获得其真实大小
        int m_nSize;
        int m_nCapacity;

        std::allocator<T> m_alloc;
    };

    template<typename T>
    DynArray<T>::DynArray(const DynArray<T>& arrElements_)
        : m_pSource(nullptr), m_nSize(0), m_nCapacity(0)
    {
        T* _pSource = m_alloc.allocate(arrElements_.m_nCapacity);
        if (_pSource == nullptr)
        {
            throw "out of memory";
        }


        std::uninitialized_copy_n(arrElements_.m_pSource, arrElements_.m_nSize, _pSource);
        m_pSource = _pSource;
        m_nSize = arrElements_.m_nSize;
        m_nCapacity = arrElements_.m_nCapacity;
    }

    template<typename T>
    DynArray<T>& DynArray<T>::operator=(const DynArray<T>& arrElements_)
    {
        T* _pSource = m_alloc.allocate(arrElements_.m_nCapacity);
        if (_pSource == nullptr)
        {
            throw "out of memory";
        }

        std::uninitialized_copy_n(arrElements_.m_pSource, arrElements_.m_nSize, _pSource);
        Free();
        m_pSource = _pSource;
        m_nSize = arrElements_.m_nSize;
        m_nCapacity = arrElements_.m_nCapacity;
        return *this;
    }

    template<typename T>
    int DynArray<T>::Find(std::function<bool(const T&)> fun_) const
    {
        int _nPosIndex = -1;
        for (int _i = 0; _i < m_nSize; _i++)
        {
            if (fun_(*(m_pSource + _i)))
            {
                _nPosIndex = _i;
                break;
            }
        }

        return _nPosIndex;
    }

    template<typename T>
    DynArray<T>::DynArray()
        :m_pSource(nullptr), m_nSize(0), m_nCapacity(0)
    {
    }

    template<typename T>
    DynArray<T>::DynArray(int nInitialSize_, const T& nInitialValue_)
        : m_pSource(nullptr), m_nSize(0), m_nCapacity(0)
    {
        T* _pSource = m_alloc.allocate(nInitialSize_ * 2);
        if (_pSource == nullptr)
        {
            throw "out of memory";
        }

        std::uninitialized_fill_n(_pSource, m_nSize, nInitialValue_);
        m_nSize = nInitialSize_;
        m_nCapacity = nInitialSize_ * 2;
        m_pSource = _pSource;
    }

    template<typename T>
    void DynArray<T>::Free()
    {
        //if(m_nSize > 0)
        //{
            for(int i = 0; i < m_nSize; i++)
            {
                m_alloc.destroy(m_pSource + i);
            }
        //}

        if(m_nCapacity > 0)
        {
            m_alloc.deallocate(m_pSource, m_nCapacity);
        }

        m_pSource = nullptr;
        m_nSize = 0;
        m_nCapacity = 0;
    }

    /*template<typename T>
    T* DynArray<T>::DeepCopy(int& nSize_)
    {
        T* _pP = (T*)malloc(sizeof(T) * m_nSize);
        if(_pP == nullptr)
        {
            nSize_ = 0;
            return _pP;
        }

        for(int i = 0; i < m_nSize; i++)
        {
            *(_pP + i) = *(m_pSource + i);
        }

        nSize_ = m_nSize;
        return _pP;
    }*/

    template<typename T>
    DynArray<T>::~DynArray()
    {
        Free();
    }

    template<typename T>
    void DynArray<T>::Add(const T& value_)
    {
        Check(m_nSize + 1);
        T* _pEnd = m_pSource + m_nSize;
        m_alloc.construct(_pEnd, value_);
        m_nSize++;
    }

    template<typename T>
    void DynArray<T>::AddRange(const DynArray& arrItems_)
    {
        Check(m_nSize + arrItems_.GetSize());
        for (int _i = 0; _i < arrItems_.m_nSize; _i++)
        {
            m_alloc.construct(m_pSource + m_nSize + _i, *(arrItems_.m_pSource + _i));
        }

        m_nSize += arrItems_.m_nSize;
    }

    template<typename T>
    void DynArray<T>::Check(int nSize_)
    {
        if (m_nCapacity >= nSize_)
        {
            return;
        }

        int _nCapacity = nSize_ * 2;
        T* _pSource = m_alloc.allocate(_nCapacity);
        if (_pSource == nullptr)
        {
            throw "out of memory";
        }


        //std::uninitialized_copy_n(m_pSource, m_nSize, _pSource);
        for(int i = 0; i < m_nSize; i++)
        {
            m_alloc.construct(_pSource + i, (*(m_pSource + i)));
        }

        int _nOldSize = m_nSize;
        Free();
        m_pSource = _pSource;
        m_nSize = _nOldSize;
        m_nCapacity = _nCapacity;
    }

    template<typename T>
    void DynArray<T>::Insert(int nIndex_, const T& value_)
    {
        if (nIndex_ > m_nSize)
        {
            throw "Insert position error";
        }

        Check(m_nSize + 1);
        m_alloc.construct(m_pSource + m_nSize, value_);


        for (int _i = m_nSize - 1; _i >= nIndex_; _i--)
        {
            *(m_pSource + _i + 1) = *(m_pSource + _i);
        }

        *(m_pSource + nIndex_) = value_;
        m_nSize++;
    }

    template<typename T>
    void DynArray<T>::DeleteByIndex(int nIndex_)
    {
        if (nIndex_ < 0
            || nIndex_ >= m_nSize)
        {
            return;
        }

        // 前移
        for (int _i = nIndex_ + 1; _i < m_nSize; _i++)
        {
            *(m_pSource + _i - 1) = *(m_pSource + _i);
        }

        m_alloc.destroy(m_pSource + m_nSize - 1);
        m_nSize--;
        if (m_nSize < m_nCapacity / 4)
        {
            Shrink();
        }
    }

    template<typename T>
    void DynArray<T>::DeleteByValue(const T& value_)
    {
        int _nIndex = Find([value_](const T& nT_)->bool
        {
            if (nT_ == value_)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

        if (_nIndex == -1)
        {
            return;
        }

        DeleteByIndex(_nIndex);
    }

    template<typename T>
    void DynArray<T>::Shrink()
    {
        int _nSize = m_nSize;
        int _nCapacity = (m_nCapacity / 2);
        T* _pSource = m_alloc.allocate(_nCapacity);
        if (_pSource == nullptr)
        {
            throw "out of memory";
        }


        std::uninitialized_copy_n(m_pSource, m_nSize, _pSource);
        Free();
        m_pSource = _pSource;
        m_nSize = _nSize;
        m_nCapacity = _nCapacity;
    }

    template<typename T>
    void DynArray<T>::DeleteAll()
    {
        Free();
    }

    template<typename T>
    bool DynArray<T>::SetValue(int nIndex_, const T& value_)
    {
        if (nIndex_ < 0
            || nIndex_ >= m_nSize)
        {
            return false;
        }

        *(m_pSource + nIndex_) = value_;
        return true;
    }

    template<typename T>
    bool DynArray<T>::GetValue(int nIndex_, T& value_) const
    {
        if (nIndex_ < 0
            || nIndex_ >= m_nSize)
        {
            return false;
        }

        value_ = *(m_pSource + nIndex_);
        return true;
    }

    template<typename T>
    int DynArray<T>::GetSize() const
    {
        return m_nSize;
    }

    template<typename T>
    int DynArray<T>::GetCapacity() const
    {
        return m_nCapacity;
    }

    template<typename T>
    T& DynArray<T>::operator[](int nIndex_)
    {
        if (nIndex_ < 0
            || nIndex_ >= m_nSize)
        {
            throw "index is error";
        }

        return *(m_pSource + nIndex_);
    }

    template<typename T>
    const T& DynArray<T>::operator[](int nIndex_) const
    {
        if (nIndex_ < 0
            || nIndex_ >= m_nSize)
        {
            throw "index is error";
        }

        return *(m_pSource + nIndex_);
    }
}

#endif // DYNARRAY_H
