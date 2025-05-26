#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include <QWidget>
#include <QScopedPointer>
#include <QString> // Required for QString parameters in signals/slots

// Forward declarations for Qt classes used as members or in Impl
// to minimize include dependencies in the header.
// However, types used in signal/slot signatures *must* be fully defined (included).
class QTabWidget;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSlider;
class QComboBox;
class QDoubleSpinBox;
class QSpinBox; // Added for kernelSizeSpinBox

class ParameterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterPanel(QWidget *parent = nullptr);
    ~ParameterPanel();

    // Public interface methods
    void setWindowLevelRange(double minVal, double maxVal);
    void setThresholdRange(double minVal, double maxVal);
    void setOpacityRange(double minVal, double maxVal);

    void setCurrentWindow(double window);
    void setCurrentLevel(double level);
    void setCurrentThreshold(double threshold);
    void setCurrentOpacity(double opacity);
    
    void updateDisplay();
    void resetParameters();

public slots:
    // Public slots callable from outside or connected to other components
    void onApplyGaussianFilter();
    void onApplyMedianFilter();
    void onApplySharpenFilter();
    void onStartRegistration();
    void onResetRegistration();
    void onClearMeasurements(); // Connected to clearMeasurementsButton

private slots:
    // Internal slots for UI element signals
    void onWindowSliderChanged(int value);
    void onLevelSliderChanged(int value);
    void onThresholdSliderChanged(int value);
    void onOpacitySliderChanged(int value);

    void onFixedImageBrowse();
    void onMovingImageBrowse();

    // Slots for new filter controls
    void onFilterTypeComboChanged(const QString& text);
    void onKernelSizeSpinBoxChanged(int value); // For QSpinBox
    void onSigmaDoubleSpinBoxChanged(double value); // For QDoubleSpinBox
    
    // Slot for new registration controls
    void onRegAlgorithmComboChanged(const QString& text);


signals:
    // Signals to notify other parts of the application
    void windowChanged(double window);
    void levelChanged(double level);
    void thresholdChanged(double lower, double upper); // Ensure this matches .cpp
    void opacityChanged(double opacity);
    
    void gaussianFilterRequested(double sigma, int kernelSize);
    void medianFilterRequested(int kernelSize);
    void sharpenFilterRequested();
    
    void registrationParametersChanged(const QString& fixedImage, const QString& movingImage, const QString& algorithm);
    void startRegistrationRequested();
    void resetRegistrationRequested();
    
    void clearMeasurementsRequested(); // Ensure this is correctly declared

    // Signals for new UI controls/parameters
    void filterTypeChanged(const QString& filterType);
    void kernelSizeChanged(int kernelSize);
    void sigmaChanged(double sigma);
    void fixedImageChanged(const QString& filePath);
    void movingImageChanged(const QString& filePath);
    void registrationAlgorithmChanged(const QString& algorithm);

private:
    // UI setup methods
    void setupUI();
    void connectSignals();

    // Tab creation methods
    QWidget* createImageControlTab();
    QWidget* createFilterProcessingTab();
    QWidget* createRegistrationTab();
    QWidget* createRenderingTab(); // Or a more descriptive name like createMeasurementAndSettingsTab

    // GroupBox creation methods (helpers for tab creation)
    QGroupBox* createImagePropertiesGroup();
    QGroupBox* createThresholdGroup();
    QGroupBox* createOpacityGroup(); 
    QGroupBox* createFilterOptionsGroup(); // Renamed for clarity
    QGroupBox* createRegistrationParamsGroup(); // Renamed for clarity
    QGroupBox* createMeasurementActionsGroup(); // Renamed for clarity

    struct Impl; // PImpl idiom
    QScopedPointer<Impl> m_impl;
};

#endif // PARAMETERPANEL_H
