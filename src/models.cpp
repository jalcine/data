/**
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

#include "md5.hpp"
#include "linguistics.hpp"
#include "models.hpp"
#include <iomanip>
#include <QFile>
#include <QMetaType>
#include <QtDebug>
#include <QMultiMap>
#include <QStringList>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <boost/progress.hpp>

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Lexical {
                Data::Data() { }

                Data::Data(const Data &p_dt) : m_id(p_dt.m_id),
                    m_lcl(p_dt.m_lcl), m_sym(p_dt.m_sym), m_flg(p_dt.m_flg) { }

                Data::Data(QString p_id, QString p_lcl, QString p_sym, DataFlagMap p_flg) : m_id(p_id),
                    m_lcl(p_lcl), m_sym(p_sym), m_flg(p_flg) { }

                const DataFlagMap Data::flags () const { return m_flg; }

                const QString Data::symbol () const { return m_sym; }

                const QString Data::id () const { return m_id; }

                const QString Data::locale () const { return m_lcl; }

                const Data Data::createData (const QString p_id, const QString p_lcl, const QString p_sym, const DataFlagMap p_flg) {
                    return Data(p_id,p_lcl,p_sym,p_flg);
                }

                Data::~Data () { }

                Model::Model() { }

                Model::Model( Data* p_dt ) : m_dt(*p_dt) { }

                Model::Model( const Model& p_mdl ) : m_dt(p_mdl.m_dt) { }

                Data* Model::data() { return &m_dt; }

                Model::~Model() { }

                LoadModel::LoadModel() { }

                LoadModel::LoadModel( Data* p_dt ) : Model(p_dt) { }

                LoadModel::LoadModel( const LoadModel& p_mdl ) : Model(p_mdl) { }

                LoadModel::LoadModel( const Model& p_mdl ) : Model(p_mdl) { }

                LoadModel::~LoadModel() { }

                SaveModel::SaveModel() { }

                SaveModel::SaveModel( Data* p_dt ) : Model(p_dt) { }

                SaveModel::SaveModel( const SaveModel& p_mdl ) : Model(p_mdl) { }

                SaveModel::SaveModel( const Model& p_mdl ) : Model(p_mdl) { }

                SaveModel::~SaveModel() { }

                Storage::Storage() { }

                Storage::Storage( Data* p_dt ) : LoadModel(p_dt), SaveModel(p_dt) { }

                Storage::Storage( const Storage& p_mdl ) : LoadModel(p_mdl), SaveModel(p_mdl) { }

                Storage* Storage::obtain ( const Data* p_data ) {
                    return NULL;
                }

                const bool Storage::exists ( const Data* p_data ) {
                    return false;
                }

                Storage::~Storage() { }
            } /** end namespace Lexical */

            namespace Rules {
                Syntax::Syntax() { }

                Syntax::Syntax(const Syntax &p_synx) : m_lcl(p_synx.m_lcl),
                    m_lnkTxt(p_synx.m_lnkTxt), m_rlTxt(p_synx.m_rlTxt) { }

                void Syntax::setLinkText (const string p_lnkTxt) { m_lnkTxt = p_lnkTxt; }

                void Syntax::setLocale (const string p_lcl) { m_lcl = p_lcl; }

                void Syntax::setRuleText (const string p_rlTxt) { m_rlTxt = p_rlTxt; }

                const string Syntax::locale () const { return m_lcl; }

                const string Syntax::ruleText () const { return m_rlTxt; }

                const string Syntax::linkText () const { return m_lnkTxt; }

                Syntax& Syntax::operator = (const Syntax& p_synx) {
                    return const_cast<Syntax&>(p_synx);
                }

                Syntax::~Syntax () { }

                Model::Model() { }

                Model::Model(const Model& p_mdl) : m_syntx(p_mdl.m_syntx) { }

                void Model::setSyntax (Syntax &p_synx) { m_syntx = p_synx; }

                const Syntax Model::syntax () const { return m_syntx; }

                Model::~Model() { }
            }
        } /** end namespace Rules */
    }
}

Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Model)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Data)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Model)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Syntax)
// kate: indent-mode cstyle; space-indent on; indent-width 4;
