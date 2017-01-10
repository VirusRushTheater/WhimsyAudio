#pragma once

#include "whimsybase.h"
#include <vector>
#include <string>
#include <sstream>

namespace whimsycore
{

/**
 * @brief Wrapper for std::vector to manage a variadic argument constructor, and nice item adder methods.
 * Due to its toString() debug method, it only can hold classes able to output their values to a STL stream, like any base classes and a whimsycore::Base derived class.
 * It will throw an error if you vectorize another, and I don't know how to make a workaround using some smart template tricks.
 */
template <typename T>
//class WhimsyVector : public Base, public std::vector<T>
class WhimsyVector : public Base
{
protected:
    std::vector<T> stl_container;

public:
    WHIMSY_OBJECT_NAME("Core/WhimsyVector")

    typedef std::vector<T> typeTvector;

    /**
     * @brief Creates an empty Vector.
     */
    WhimsyVector()
    {
    }

    virtual ~WhimsyVector()
    {
    }

    /**
     * @brief Creates a vector, and adds all the supplied arguments as new elements of this vector.
     * @param firstarg  Argument to ensure this constructor is called with, at least, one element.
     * @param others    Variadic parameter list. Make sure all elements of this list are the same type of this vector.
     */
    template<typename ... Args>
    WhimsyVector(const T& firstarg, Args ... variadicargs)
    {
        addItems(firstarg, variadicargs...);
    }

    /**
     * @brief Adds all the supplied arguments as new elements of this vector.
     * @param elem      Argument to ensure this method is called with, at least, one element.
     * @param others    Variadic parameter list. Make sure all elements of this list are the same type of this vector.
     * @return Reference to this vector.
     */
    template<typename ... Args>
    WhimsyVector<T>& addItems(const T& elem, Args ... others)
    {
        this->push_back(elem);
        return addItems(others...);
    }

    /**
     * @brief End for the addItems recursive algorithm. Does not do anything.
     * @return  Reference to this vector.
     */
    WhimsyVector<T>& addItems()
    {
        return *this;
    }

    /**
     * @brief Copies the elements of the vector `vec` into the end of this vector.
     * @param vec   Vector with elements to concatenate.
     * @return      Reference to this vector.
     */
    WhimsyVector<T>& concatenate(const WhimsyVector<T>& vec)
    {
        this->insert(this->end(), vec.begin(), vec.end());
        return *this;
    }

    /**
     * @brief Returns a byte array holding raw data for this vector.
     * @return
     */
    byte* lowLevelData()
    {
        return reinterpret_cast<byte*>(&(this->at(0)));
    }

    /**
     * @brief Returns how many bytes length is the data held by this vector. To use in conjunction with lowLevelData().
     * @return
     */
    size_t lowLevelDataLength() const
    {
        return this->size() * sizeof(T);
    }

    /**
     * @brief Enables using the << operator to add elements to the vector. Equivalent to push_back().
     * @param elem  New element for this vector.
     * @return      Reference to this vector.
     */
    WhimsyVector<T>& operator << (const T&& elem)
    {
        this->push_back(elem);
        return *this;
    }

    /**
     * @brief Alias for concatenate.
     * @param       Vector with elements to concatenate.
     * @return      Reference to this vector.
     */
    WhimsyVector<T>& operator << (const WhimsyVector<T>& vec)
    {
        return this->concatenate(vec);
    }

    /**
     * @brief Outputs a Python-like representation of this vector.
     * @return String representation of this vector.
     */
    std::string toString() const
    {
        std::ostringstream              retval;
        size_t                          cit;

        retval << "[";
        for(cit = 0; cit < this->size(); cit++)
        {
            retval << this->at(cit);
            if(cit != this->size() - 1)
                retval << ", ";
        }
        retval << "]";

        return retval.str();
    }

    // *********************************************************************************
    // STL vector replacement methods.

    size_t size() const
    {
        return stl_container.size();
    }

    void push_back(const T& element)
    {
        stl_container.push_back(element);
    }

    T* end() const
    {
        return ((T*)&(stl_container[0])) + stl_container.size();
    }

    T* begin() const
    {
        return ((T*)&(stl_container[0]));
    }

    void resize(size_t newsize)
    {
        stl_container.resize(newsize);
    }

    T& at(size_t index)
    {
        return stl_container.at(index);
    }

    const T& at(size_t index) const
    {
        return stl_container.at(index);
    }

    T& operator [] (size_t index)
    {
        return stl_container[index];
    }

    const T& operator [] (size_t index) const
    {
        return stl_container.at(index);
    }

    void clear()
    {
        stl_container.clear();
    }

    template<typename Tin>
    T* insert(T* container_position, const Tin* incoming_begin, const Tin* incoming_end)
    {
        size_t offset = container_position - begin();
        T* incoming_begin_T = (T*)incoming_begin;
        T* incoming_end_T = (T*)incoming_end;
        size_t difference = incoming_end_T - incoming_begin_T;

        stl_container.insert(stl_container.begin() + offset, incoming_begin_T, incoming_end_T);

        return &(container_position[offset + difference]);
    }
};

}
