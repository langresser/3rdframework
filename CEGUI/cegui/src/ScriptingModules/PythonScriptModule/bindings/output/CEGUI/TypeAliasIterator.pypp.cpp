// This file has been generated by Py++.

#include "boost/python.hpp"
#include "python_CEGUI.h"
#include "TypeAliasIterator.pypp.hpp"

namespace bp = boost::python;

void Iterator_next(::CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >& t)
{
    t++;
}

void Iterator_previous(::CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >& t)
{
    t--;
}

void register_TypeAliasIterator_class(){

    { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >
        typedef bp::class_< CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > > TypeAliasIterator_exposer_t;
        TypeAliasIterator_exposer_t TypeAliasIterator_exposer = TypeAliasIterator_exposer_t( "TypeAliasIterator", bp::no_init );
        bp::scope TypeAliasIterator_scope( TypeAliasIterator_exposer );
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::getCurrentKey
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef ::CEGUI::String ( exported_class_t::*getCurrentKey_function_type )(  ) const;
            
            TypeAliasIterator_exposer.def( 
                "getCurrentKey"
                , getCurrentKey_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::getCurrentKey )
                , "*!\n\
            \n\
               Return the key for the item at the current iterator position.\n\
            *\n" );
        
        }
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::getCurrentValue
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef ::CEGUI::WindowFactoryManager::AliasTargetStack ( exported_class_t::*getCurrentValue_function_type )(  ) const;
            
            TypeAliasIterator_exposer.def( 
                "getCurrentValue"
                , getCurrentValue_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::getCurrentValue )
                , "*!\n\
            \n\
               Return the value for the item at the current iterator position.\n\
            *\n" );
        
        }
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::isAtEnd
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef bool ( exported_class_t::*isAtEnd_function_type )(  ) const;
            
            TypeAliasIterator_exposer.def( 
                "isAtEnd"
                , isAtEnd_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::isAtEnd )
                , "*!\n\
            \n\
               Return whether the current iterator position is at the end of the iterators range.\n\
            *\n" );
        
        }
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::isAtStart
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef bool ( exported_class_t::*isAtStart_function_type )(  ) const;
            
            TypeAliasIterator_exposer.def( 
                "isAtStart"
                , isAtStart_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::isAtStart )
                , "*!\n\
            \n\
               Return whether the current iterator position is at the start of the iterators range.\n\
            *\n" );
        
        }
        TypeAliasIterator_exposer.def( bp::self != bp::self );
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::operator=
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef ::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > & ( exported_class_t::*assign_function_type )( ::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > const & ) ;
            
            TypeAliasIterator_exposer.def( 
                "assign"
                , assign_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::operator= )
                , ( bp::arg("rhs") )
                , bp::return_self< >()
                , "*!\n\
            \n\
               ConstBaseIterator assignment operator\n\
            *\n" );
        
        }
        TypeAliasIterator_exposer.def( bp::self == bp::self );
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::toEnd
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef void ( exported_class_t::*toEnd_function_type )(  ) ;
            
            TypeAliasIterator_exposer.def( 
                "toEnd"
                , toEnd_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::toEnd )
                , "*!\n\
            \n\
               Set the iterator current position to the end position.\n\
            *\n" );
        
        }
        { //::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::toStart
        
            typedef CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > > exported_class_t;
            typedef void ( exported_class_t::*toStart_function_type )(  ) ;
            
            TypeAliasIterator_exposer.def( 
                "toStart"
                , toStart_function_type( &::CEGUI::ConstBaseIterator< std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<CEGUI::String const, CEGUI::WindowFactoryManager::AliasTargetStack> > > >::toStart )
                , "*!\n\
            \n\
               Set the iterator current position to the start position.\n\
            *\n" );
        
        }
        TypeAliasIterator_exposer.def("next", &::Iterator_next);
        TypeAliasIterator_exposer.def("previous", &::Iterator_previous);
    }

}