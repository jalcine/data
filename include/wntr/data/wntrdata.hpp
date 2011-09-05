/**
 * @file wntrdata.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
/**
 * @mainpage Wintermute Data (WntrData) Documentation
 * Wintermute relies on the aggregation and pooling of data resources
 * to make sense of the (possibly) ten of hundreds of nodes of information
 * it recieves. There's dedicated systems to obtaining information regarding
 * syntax and linguistics for natural language processing (Wintermute::Data::Linguistics)
 * and ontological resources (Wintermute::Data::Ontology).
 * Currently, the language data system provides an abstract class system so that
 * plug-in developers can implement new and foriegn sources of data for lexical
 * and syntactic binding. Plug-in developers are encouraged to focus on data sources
 * that introduce new sources of words aside from importing new syntactic information
 * for the fact the syntactic system still needs a bit more work before it becomes as
 * decently mature as the lexical system.
 */
#ifndef __WNTRDATA_HPP__
#define __WNTRDATA_HPP__

#include "config.hpp"
#include "ontology.hpp"
#include "linguistics.hpp"
#include "models.hpp"

namespace Wintermute {
    namespace Data {

    }
}

#endif /* __WNTRDATA_HPP__ */
// kate: indent-mode cstyle; space-indent on; indent-width 4;
