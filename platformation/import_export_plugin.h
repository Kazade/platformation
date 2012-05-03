/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify 
*  it under the terms of the GNU Lesser General Public License as published 
*  by the Free Software Foundation; either version 3 of the License, or (at 
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but 
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License 
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#ifndef IMPORT_EXPORT_PLUGIN_H_INCLUDED
#define IMPORT_EXPORT_PLUGIN_H_INCLUDED

class ImportExportPlugin {
public:
    virtual ~ImportExportPlugin() = 0;
    virtual bool save_to(const std::string& filename, const Level* level) = 0;
    virtual bool load_from(const std::string& filename, Level* level) = 0;
    virtual std::string get_extension() = 0;
};

typedef ImportExportPlugin* CreateImportExportFunc();
typedef void DestroyImportExportFunc(ImportExportPlugin*);

#endif // IMPORT_EXPORT_PLUGIN_H_INCLUDED
