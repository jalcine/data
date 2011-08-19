/**
 * @file ontology.hpp
 * @author Jacky Alcine
 * @date March 29, 2011, 2:25 PM
 * @namespace Wintermute::Data::Ontology
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

#ifndef ONTOLOGY_HPP
#define	ONTOLOGY_HPP

#include <QMap>
#include <QString>
#include <Soprano/Soprano>

using namespace std;
using namespace Soprano;

using std::map;
using std::pair;
using std::string;
using Soprano::Model;

namespace Wintermute {
    namespace Data {
        namespace Ontology {
            struct Configuration;
            struct Concept;
            struct Knowledge;
            struct Repository;

            /**
             * @brief Management class for the ontologies.
             * This class manages every and anything having to deal with ontologies.
             * @class Configuration ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Configuration : public QObject {
                Q_OBJECT

                public:
                    /**
                     * @brief Initializes the ontology system.
                     * Runs all of the necessary initialziation code to get the ontology system on its toes.
                     * @fn Initialize
                     */
                    Q_INVOKABLE static void Initialize();
                    /**
                     * @brief Deinitializes the ontology system/
                     * Runs all of the necessary deinitialization code to have the ontology system be safely shut down.
                     * @fn Deinitialize
                     */
                    Q_INVOKABLE static void Deinitialize();
            };

            /**
             * @brief Represents a bit of data.
             * Concepts are the most basic components of an ontology.
             * @class Concept ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Concept : public QObject { };

            /**
             * @brief Represents a link of concepts.
             * @c Knowledge objects represent Concepts made useful; by giving it a sense of meaning.
             * @class Knowledge ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Knowledge : public QObject { };

            /**
             * @brief Represents a collection of @c Knowledge.
             * @c Repository
             * @class Repository ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Repository : public QObject { };
        }
    }
}

#endif	/* ONTOLOGY_HPP */

// kate: indent-mode cstyle; space-indent on; indent-width 4;
