/**
 * @file linguistics.cpp
 * @author Jacky Alcine <jackyalcine@gmail.com>
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

#include <map>
#include <string>
#include <QFile>
#include <QDomDocument>
#include <iostream>
#include <QTextStream>
#include <boost/filesystem.hpp>
#include <boost/serialization/base_object.hpp>
#include "md5.hpp"
#include "config.hpp"
#include "linguistics.hpp"
#include "models.hpp"

using namespace std;
using namespace boost::serialization;
using namespace boost::archive;
using namespace Wintermute::Data;

using std::string;
using boost::archive::xml_iarchive;
using boost::archive::xml_oarchive;

namespace Wintermute {
	 namespace Data {
	  namespace Linguistics {
		   string Configuration::_storageDir = string("./") + string(WNTRDATA_LING_DIR);
		   string Configuration::_locale = "en";

		   void Configuration::Initialize(const string storageDir, const string locale)
		   {
			//cout << "(data) [Ling:Config] Initializing data.." << endl;
			Configuration::setDirectory(storageDir);
			Configuration::setLocale(locale);

			//cout << "(data) [Ling:Config] Setting up linguistics data sources... " << endl;
			Storage::addDataSource(LocalStorage::create,LocalStorage::exists);
			Storage::addDataSource(XMLStorage::create,XMLStorage::exists);

			//cout << "(data) [Ling:Config] Parsing sources.. " << endl;
			LocalStorage::spawn();
			XMLStorage::spawn();
		   }

		   void Configuration::Deinitialize(){

		   }

		   void Configuration::setLocale(string const locale)
		   {
			if (locale.size() == 0)
			 return;

			Configuration::_locale = locale;
			cout << "(data) [Ling:Config] ## Global locale set to " << locale << endl;
		   }

		   void Configuration::setDirectory(string const configDir)
		   {
			if (configDir.size() == 0)
			 return;

			boost::filesystem::path newConfigDir(configDir);
			Configuration::_storageDir = newConfigDir.string();

			cout << "(data) [Ling:Config] ## Root directory set to " << configDir << endl;
		   }
	  } // namespaces
	 }
}
