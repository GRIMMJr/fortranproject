/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef WORKSPACEBROWSERBUILDER_H
#define WORKSPACEBROWSERBUILDER_H

#include <wx/thread.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>

#include "tokenf.h"
#include "parserf.h"

typedef std::map<std::string,int> StrIntMap;

enum SpecialFolder
{
    sfToken         = 0x0001, // token node
    sfRoot          = 0x0002, // root node
    sfGFuncs        = 0x0004, // global funcs node
    sfOthers       = 0x0010, // preprocessor symbols node
};

enum BrowserDisplayFilter
{
    bdfFile = 0,
    bdfProject,
    bdfWorkspace,
};

struct BrowserOptions
{
    BrowserDisplayFilter displayFilter; // default: bdfWorkspace
    bool visibleBottomTree;
    bool sortAlphabetically;
};

class TreeDataF : public wxTreeItemData
{
    public:
        TreeDataF(SpecialFolder sf = sfToken, TokenF* token = 0)
            : m_pToken(token),
            m_SpecialFolder(sf)
        {
        }
        TokenF* m_pToken;
        SpecialFolder m_SpecialFolder;
};

class WorkspaceBrowserBuilder
{
    public:
        WorkspaceBrowserBuilder(ParserF* parser, wxTreeCtrl* treeTop, wxTreeCtrl* treeBottom);
        virtual ~WorkspaceBrowserBuilder();

        void Init(const wxString& active_filename,
                    cbProject* active_project,
                    const BrowserOptions& options);
        void ExpandTop();
        void ExpandTopNode(wxTreeItemId node);
        void CollapsTopNode(wxTreeItemId node);
        void SelectItem(TokenF* item);
        void SelectNode(wxTreeItemId node);
        void DeleteTopRootChildren();
        wxImageList* GetImageList();
		int GetTokenKindImageIdx(TokenF* token);
		void MarkSymbol(const wxString& filename, int line);


		bool m_AtWork;

    protected:
        void BuildTree();
        wxTreeItemId AddNodeIfNotThere(wxTreeCtrl* tree, wxTreeItemId parent, const wxString& name, int imgIndex = -1, TreeDataF* data = 0, bool sorted = true);
        void AddTreeChildren(wxTreeCtrl* tree, wxTreeItemId parent, int tokenKindMask);
        bool AddFileNodes(wxTreeCtrl* tree, wxTreeItemId parent, wxString file, int tokenKindMask);
        bool AddChildrenNodes(wxTreeCtrl* tree, wxTreeItemId parent, TokenF* token, int tokenKindMask);
        int AddInterfaceNode(wxTreeCtrl* tree, wxTreeItemId parent, TokenF* parToken);
        wxTreeItemId AddTreeNode(wxTreeCtrl* tree, const wxTreeItemId& parentNode, TokenF* token);
        wxTreeItemId FindItemByName(wxTreeCtrl* tree, wxString name, wxString name2=wxString());
        void CreateSpecialFolders();
        bool FileHasTokens(const wxString& fileName, int tokenKindMask);
        bool HasGlobalFunctionsOthers(int tokenKindMask);
        bool HasChildren(TokenF* tokenParent, int tokenKindMask);
        void MarkItem(wxTreeCtrl* tree, wxTreeItemId& item, bool mark=true);
        bool MarkBottomSymbol(const wxString& filename, int line);
        void UnmarkBottomSymbol();
        void MarkChildSymbol(wxTreeCtrl* tree, wxTreeItemId& root, int line, bool mark=true);
        bool MarkGlobalSymbol(wxTreeCtrl* tree, wxTreeItemId& root, const wxString& filename, int line);
        bool IsLineInGlobals(const wxString& file, int line);

        ParserF* m_pParser;
        wxTreeCtrl* m_pTreeTop;
        wxTreeCtrl* m_pTreeBottom;
        wxImageList* m_pImlist;
        StrIntMap m_ImgNr;

        wxString m_ActiveFilename;
        cbProject* m_pActiveProject;
        BrowserOptions m_Options;
        wxArrayString m_ExpandedNodes;

    private:
};

#endif // WORKSPACEBROWSERBUILDER_H
