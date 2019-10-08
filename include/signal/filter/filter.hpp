/**
  ******************************************************************************
  * @file    Filter.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the filter
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef FILTER_HPP
#define FILTER_HPP

#include <utils/linalg/linalg.h>
#include <signal/systemmodels/systemmodels.hpp>

namespace signal::filter
{
    /**
     * @brief The filter interface
     * 
     * @tparam T The type of the input and output signal
     */
    template<class T>
    class IFilter
    {
        public:
            virtual T operator()(T&)=0;
    }; // Class IFilter

    namespace lti
    {
        namespace siso
        {

            /**
             * @brief Infinite impulse response (IIR) discrete-time filter template class
             * 
             * @tparam T    The type of the input and output signal
             * @tparam NA   Number of coefficients for feedback filter
             * @tparam NB   Number of coefficients for feedforward filter
             */
            template <class T, uint32_t NA, uint32_t NB>
            class CIIRFilter:public IFilter<T>
            {
                public:
                    /* Constructor */
                    CIIRFilter(const utils::linalg::CRowVector<T,NA>& f_A,const utils::linalg::CRowVector<T,NB>& f_B);
                    /* Operator */
                    T operator()(T& f_u);
                private:
                    CIIRFilter(){}
                    utils::linalg::CRowVector<T,NA> m_A;
                    utils::linalg::CRowVector<T,NB> m_B;
                    utils::linalg::CColVector<T,NA> m_Y;
                    utils::linalg::CColVector<T,NB> m_U;
            }; // class CIIRFilter

            /**
             * @brief Finite impulse response (FIR) discrete-time filter.
             * 
             * @tparam T    The type of the input and output signal
             * @tparam NB   Number of coefficients for feedforward filter
             */
            template <class T, uint32_t NB>
            class CFIRFilter:public IFilter<T>
            {
                public:
                    /* Constructor */
                    CFIRFilter(const utils::linalg::CRowVector<T,NB>& f_B);
                    /* Operator */
                    T operator()(T& f_u);
                private:
                    CFIRFilter() {}
                    utils::linalg::CRowVector<T,NB> m_B;
                    utils::linalg::CColVector<T,NB> m_U;
            }; // class CFIRFilter

            /**
             * @brief Mean filter
             * 
             * @tparam T    The type of the input and output signal
             * @tparam NB   Number of memorized values for calculating mean value
             */
            template <class T, uint32_t NB>
            class CMeanFilter:public IFilter<T>
            {
                public:
                    /* Constructor */
                    CMeanFilter();
                    /* Operator */
                    virtual T operator()(T& f_u);
                private:
                    T m_B;
                    utils::linalg::CColVector<T,NB> m_U;
            }; // class CMeanFilter
        }; // namespace siso
    }; // namespace linear


    namespace nlti
    {
        namespace siso
        {   

            /**
             * @brief  Median filter 
             * 
             * @tparam T        type of the values
             * @tparam N        size of the filter
             */
            template <class T, uint32_t N>
            class CMedianFilter:public IFilter<T>
            {
            public:
                /* Constructor */
                CMedianFilter();
                inline T addNewValue(T& f_val);
                inline T getMedian();
                inline T operator()(T& f_v);

            private:
                struct my_structure
                {
                    T info;
                      my_structure *next;
                      my_structure *prev;
                    
                };

                my_structure *m_median;    //pointer to the loist element that represents the median
                my_structure *m_smallest;  //pointer to the smallest element of the list (the first)
                uint32_t m_new;		     //index to the newest element in the list
                uint32_t m_size;		     //the size of the list/queue

                my_structure m_queue[N];
            }; // class CMedianFilter
        }; // namespace siso
    }; // namespace nonlinear 
}; // namespace singal::filter

#include "filter.tpp"

#endif // FILTER_H