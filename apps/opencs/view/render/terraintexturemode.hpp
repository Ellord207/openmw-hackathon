#ifndef CSV_RENDER_TERRAINTEXTUREMODE_H
#define CSV_RENDER_TERRAINTEXTUREMODE_H

#include "editmode.hpp"

#include <string>

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QSlider>
#include <QIcon>
#include <QFrame>
#include <QEvent>
#include <QHBoxLayout>
#include <QPushButton>

#include "../../model/world/data.hpp"
#include "../../model/world/land.hpp"

#include "../../model/doc/document.hpp"
#include "../../model/world/commands.hpp"
#include "../../model/world/idtable.hpp"
#include "../../model/world/landtexture.hpp"

namespace CSVWidget
{
    class SceneToolMode;
}

namespace CSVRender
{

    /// \brief Layout-box for some brush button settings
    class BrushSizeControls : public QGroupBox
    {
        Q_OBJECT

        public:
            BrushSizeControls(const QString &title, QWidget *parent);
            QSlider *brushSizeSlider;

        private:
            QSpinBox *brushSizeSpinBox;
            QHBoxLayout *layoutSliderSize;
    };

    /// \brief Brush settings window
    class TextureBrushWindow : public QFrame
    {
        Q_OBJECT

        public:
            TextureBrushWindow(WorldspaceWidget *worldspaceWidget, QWidget *parent = 0);
            void configureButtonInitialSettings(QPushButton *button);

            QPushButton *buttonPoint = new QPushButton(QIcon (QPixmap (":scenetoolbar/brush-point")), "", this);
            QPushButton *buttonSquare = new QPushButton(QIcon (QPixmap (":scenetoolbar/brush-square")), "", this);
            QPushButton *buttonCircle = new QPushButton(QIcon (QPixmap (":scenetoolbar/brush-circle")), "", this);
            QPushButton *buttonCustom = new QPushButton(QIcon (QPixmap (":scenetoolbar/brush-custom")), "", this);

        private:
            QLabel *selectedBrush;
            QGroupBox *horizontalGroupBox;
            WorldspaceWidget *mWorldspaceWidget;
            int mBrushSize;
            int mBrushShape;
            std::string mBrushTexture;
            std::string mBrushTextureLabel;

        public slots:
            void setBrushTexture(std::string brushTexture);
            void setBrushShape();
            void setBrushSize(int brushSize);

        signals:
            void passBrushSize (int brushSize);
            void passBrushShape(int brushShape);
    };

    class TerrainTextureMode : public EditMode
    {
        Q_OBJECT

        public:

            /// \brief Editmode for terrain texture grid
            TerrainTextureMode(WorldspaceWidget*, QWidget* parent = nullptr);

            /// \brief Create single command for one-click texture editing
            void primaryEditPressed (const WorldspaceHitResult& hit);

            /// \brief Open brush settings window
            void primarySelectPressed(const WorldspaceHitResult&);

            void secondarySelectPressed(const WorldspaceHitResult&);

            void activate(CSVWidget::SceneToolbar*);
            void deactivate(CSVWidget::SceneToolbar*);

            /// \brief Start texture editing command macro
            virtual bool primaryEditStartDrag (const QPoint& pos);

            virtual bool secondaryEditStartDrag (const QPoint& pos);
            virtual bool primarySelectStartDrag (const QPoint& pos);
            virtual bool secondarySelectStartDrag (const QPoint& pos);

            /// \brief Handle texture edit behavior during dragging
            virtual void drag (const QPoint& pos, int diffX, int diffY, double speedFactor);

            /// \brief End texture editing command macro
            virtual void dragCompleted(const QPoint& pos);

            virtual void dragAborted();
            virtual void dragWheel (int diff, double speedFactor);
            virtual void dragMoveEvent (QDragMoveEvent *event);

            /// \brief Handle brush mechanics, maths regarding worldspace hit etc.
            void editTerrainTextureGrid (const WorldspaceHitResult& hit);

            /// \brief Push texture edits to command macro
            void pushEditToCommand (CSMWorld::LandTexturesColumn::DataType& newLandGrid, CSMDoc::Document& document,
                CSMWorld::IdTable& landTable, std::string cellId);

            /// \brief Create new land texture record from texture asset
            void createTexture(std::string textureFileName);

        private:
            TextureBrushWindow *textureBrushWindow;
            std::string mCellId;
            std::string mBrushTexture;
            int mBrushSize;
            int mBrushShape;

            const int cellSize {ESM::Land::REAL_SIZE};
            const int landSize {ESM::Land::LAND_SIZE};
            const int landTextureSize {ESM::Land::LAND_TEXTURE_SIZE};

        signals:
            void passBrushTexture(std::string brushTexture);

        public slots:
            void handleDragEnterEvent (QDragEnterEvent *event);
            void handleDropEvent(QDropEvent *event);
            void handleMouseEvent (QMouseEvent *event);
            void handlePrimarySelectOnModeButton();
            void setBrushSize(int brushSize);
            void setBrushShape(int brushShape);

    };
}


#endif
