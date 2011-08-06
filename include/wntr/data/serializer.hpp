/**
 * @file serializer.hpp
 * @author Adrian Borucki
 * @legalese
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 */

#ifndef __SERIALIZER_HPP__
#define __SERIALIZER_HPP__

#include <vector>
#include <fstream>
#include "models.hpp"

using namespace std;

using std::vector;
using std::ofstream;

namespace Wintermute
{
    namespace Data
    {
        /**
         * @brief Class for generic objects serialization.
         * This class is a base for every class that provides serialization capabilities.
         * @todo Think about serializers managing, maybe as plugins or something.
         */
        class Serializer
        {
        public:
            /**
             * @brief
             *
             * @param it
             * @param outStream
             */
            virtual void serialize(Linguistics::LLexicalIterator it, ofstream& outStream);
            /**
             * @brief
             *
             * @fn serialize
             * @param it
             * @param outStream
             */
            virtual void serialize(Linguistics::XLexicalIterator it, ofstream& outStream);
        };

    } // namespaces
}

#endif // __SERIALIZER_HPP__
