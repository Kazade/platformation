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
