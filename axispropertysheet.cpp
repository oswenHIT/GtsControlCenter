#include "axispropertysheet.h"

AxisPropertySheet::AxisPropertySheet(QWidget *parent)
	: QWidget(parent)
{
	propertyBrowser = new QtTreePropertyBrowser;
	loadSetting = new QPushButton;
	resetButton = new QPushButton;
	loadSetting->setText(tr("Load Settings"));
	resetButton->setText(tr("Reset Settings"));

	createPropertySheet();

	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addWidget(loadSetting);
	hlayout->addWidget(resetButton);

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addWidget(propertyBrowser);
	vlayout->addLayout(hlayout);
	vlayout->setContentsMargins(0, 1, 0, 0);
	
	setLayout(vlayout);
}

AxisPropertySheet::~AxisPropertySheet()
{

}

void AxisPropertySheet::createPropertySheet()
{

	QtGroupPropertyManager
		*axisGroupManager = new QtGroupPropertyManager;
	QtProperty *axisOne = axisGroupManager->addProperty(tr("Axis One"));
	QtProperty *axisTwo = axisGroupManager->addProperty(tr("Axis Two"));
	QtProperty *axisThree = axisGroupManager->addProperty(tr("Axis Three"));
	QtProperty *axisFour = axisGroupManager->addProperty(tr("Axis Four"));

	QtBoolPropertyManager *boolManager = new QtBoolPropertyManager;
	QtCheckBoxFactory *checkBox = new QtCheckBoxFactory;
	QtProperty *axisOneActivate = boolManager->addProperty(tr("Activated"));
	QtProperty *axisTwoActivate = boolManager->addProperty(tr("Activated"));
	QtProperty *axisThreeActivate = boolManager->addProperty(tr("Activated"));
	QtProperty *axisFourActivate = boolManager->addProperty(tr("Activated"));
	QtProperty *driveAlertActivate_1 = boolManager->addProperty(tr("Activated"));
	QtProperty *driveAlertActivate_2 = boolManager->addProperty(tr("Activated"));
	QtProperty *driveAlertActivate_3 = boolManager->addProperty(tr("Activated"));
	QtProperty *driveAlertActivate_4 = boolManager->addProperty(tr("Activated"));
	QtProperty *posLimitActivate_1 = boolManager->addProperty(tr("Activated"));
	QtProperty *posLimitActivate_2 = boolManager->addProperty(tr("Activated"));
	QtProperty *posLimitActivate_3 = boolManager->addProperty(tr("Activated"));
	QtProperty *posLimitActivate_4 = boolManager->addProperty(tr("Activated"));
	QtProperty *negLimitActivate_1 = boolManager->addProperty(tr("Activated"));
	QtProperty *negLimitActivate_2 = boolManager->addProperty(tr("Activated"));
	QtProperty *negLimitActivate_3 = boolManager->addProperty(tr("Activated"));
	QtProperty *negLimitActivate_4 = boolManager->addProperty(tr("Activated"));
	QtProperty *gOActivated_1 = boolManager->addProperty(tr("Activated"));
	QtProperty *gOActivated_2 = boolManager->addProperty(tr("Activated"));
	QtProperty *gOActivated_3 = boolManager->addProperty(tr("Activated"));
	QtProperty *gOActivated_4 = boolManager->addProperty(tr("Activated"));
	QtProperty *servoEnableActivated_1 = boolManager->addProperty(tr("Activated"));
	QtProperty *servoEnableActivated_2 = boolManager->addProperty(tr("Activated"));
	QtProperty *servoEnableActivated_3 = boolManager->addProperty(tr("Activated"));
	QtProperty *servoEnableActivated_4 = boolManager->addProperty(tr("Activated"));
	QtProperty *statClearActivated_1 = boolManager->addProperty(tr("Activated"));
	QtProperty *statClearActivated_2 = boolManager->addProperty(tr("Activated"));
	QtProperty *statClearActivated_3 = boolManager->addProperty(tr("Activated"));
	QtProperty *statClearActivated_4 = boolManager->addProperty(tr("Activated"));
	QtProperty *driveAlertInvert_1 = boolManager->addProperty(tr("Inverted"));
	QtProperty *driveAlertInvert_2 = boolManager->addProperty(tr("Inverted"));
	QtProperty *driveAlertInvert_3 = boolManager->addProperty(tr("Inverted"));
	QtProperty *driveAlertInvert_4 = boolManager->addProperty(tr("Inverted"));
	QtProperty *posLimitInvert_1 = boolManager->addProperty(tr("Inverted"));
	QtProperty *posLimitInvert_2 = boolManager->addProperty(tr("Inverted"));
	QtProperty *posLimitInvert_3 = boolManager->addProperty(tr("Inverted"));
	QtProperty *posLimitInvert_4 = boolManager->addProperty(tr("Inverted"));
	QtProperty *negLimitInvert_1 = boolManager->addProperty(tr("Inverted"));
	QtProperty *negLimitInvert_2 = boolManager->addProperty(tr("Inverted"));
	QtProperty *negLimitInvert_3 = boolManager->addProperty(tr("Inverted"));
	QtProperty *negLimitInvert_4 = boolManager->addProperty(tr("Inverted"));
	QtProperty *gOInverted_1 = boolManager->addProperty(tr("Inverted"));
	QtProperty *gOInverted_2 = boolManager->addProperty(tr("Inverted"));
	QtProperty *gOInverted_3 = boolManager->addProperty(tr("Inverted"));
	QtProperty *gOInverted_4 = boolManager->addProperty(tr("Inverted"));
	QtProperty *servoEnableInverted_1 = boolManager->addProperty(tr("Inverted"));
	QtProperty *servoEnableInverted_2 = boolManager->addProperty(tr("Inverted"));
	QtProperty *servoEnableInverted_3 = boolManager->addProperty(tr("Inverted"));
	QtProperty *servoEnableInverted_4 = boolManager->addProperty(tr("Inverted"));
	QtProperty *statClearInverted_1 = boolManager->addProperty(tr("Inverted"));
	QtProperty *statClearInverted_2 = boolManager->addProperty(tr("Inverted"));
	QtProperty *statClearInverted_3 = boolManager->addProperty(tr("Inverted"));
	QtProperty *statClearInverted_4 = boolManager->addProperty(tr("Inverted"));
	axisOne->addSubProperty(axisOneActivate);
	axisTwo->addSubProperty(axisTwoActivate);
	axisThree->addSubProperty(axisThreeActivate);
	axisFour->addSubProperty(axisFourActivate);
	propertyBrowser->setFactoryForManager(boolManager, checkBox);

	QtEnumPropertyManager *enumManager = new QtEnumPropertyManager;
	QtEnumEditorFactory *enumEditor = new QtEnumEditorFactory;
	QStringList axisModeTypes;
	axisModeTypes << "CCW/CW" << "Pulse+Direction";
	QtProperty *axisModeOne = enumManager->addProperty(tr("Axis Mode"));
	QtProperty *axisModeTwo = enumManager->addProperty(tr("Axis Mode"));
	QtProperty *axisModeThree = enumManager->addProperty(tr("Axis Mode"));
	QtProperty *axisModeFour = enumManager->addProperty(tr("Axis Mode"));
	enumManager->setEnumNames(axisModeOne, axisModeTypes);
	enumManager->setValue(axisModeOne, 0);
	enumManager->setEnumNames(axisModeTwo, axisModeTypes);
	enumManager->setValue(axisModeTwo, 0);
	enumManager->setEnumNames(axisModeThree, axisModeTypes);
	enumManager->setValue(axisModeThree, 0);
	enumManager->setEnumNames(axisModeFour, axisModeTypes);
	enumManager->setValue(axisModeFour, 0);
	propertyBrowser->setFactoryForManager(enumManager, enumEditor);
	axisOne->addSubProperty(axisModeOne);
	axisTwo->addSubProperty(axisModeTwo);
	axisThree->addSubProperty(axisModeThree);
	axisFour->addSubProperty(axisModeFour);

	QtGroupPropertyManager
		*axisGroupDigitalInput = new QtGroupPropertyManager;
	QtProperty *axisOneDigitalInput = axisGroupDigitalInput->addProperty(tr("Axis Digital Input"));
	axisOne->addSubProperty(axisOneDigitalInput);
	QtProperty *axisTwoDigitalInput = axisGroupDigitalInput->addProperty(tr("Axis Digital Input"));
	axisTwo->addSubProperty(axisTwoDigitalInput);
	QtProperty *axisThreeDigitalInput = axisGroupDigitalInput->addProperty(tr("Axis Digital Input"));
	axisThree->addSubProperty(axisThreeDigitalInput);
	QtProperty *axisFourDigitalInput = axisGroupDigitalInput->addProperty(tr("Axis Digital Input"));
	axisFour->addSubProperty(axisFourDigitalInput);

	QtGroupPropertyManager
		*axisGroupDigitalOutput = new QtGroupPropertyManager;
	QtProperty *axisOneDigitalOutput = axisGroupDigitalOutput->addProperty(tr("Axis Digital Output"));
	axisOne->addSubProperty(axisOneDigitalOutput);
	QtProperty *axisTwoDigitalOutput = axisGroupDigitalOutput->addProperty(tr("Axis Digital Output"));
	axisTwo->addSubProperty(axisTwoDigitalOutput);
	QtProperty *axisThreeDigitalOutput = axisGroupDigitalOutput->addProperty(tr("Axis Digital Output"));
	axisThree->addSubProperty(axisThreeDigitalOutput);
	QtProperty *axisFourDigitalOutput = axisGroupDigitalOutput->addProperty(tr("Axis Digital Output"));
	axisFour->addSubProperty(axisFourDigitalOutput);

	QtGroupPropertyManager
		*axisSubInputGroup = new QtGroupPropertyManager;
	QtProperty *axisOneDriveAlert = axisSubInputGroup->addProperty(tr("Drive Alert"));
	QtProperty *axisOnePosLimit = axisSubInputGroup->addProperty(tr("Positive Limit"));
	QtProperty *axisOneNegLimit = axisSubInputGroup->addProperty(tr("Negative Limit"));
	QtProperty *axisTwoDriveAlert = axisSubInputGroup->addProperty(tr("Drive Alert"));
	QtProperty *axisTwoPosLimit = axisSubInputGroup->addProperty(tr("Positive Limit"));
	QtProperty *axisTwoNegLimit = axisSubInputGroup->addProperty(tr("Negative Limit"));
	QtProperty *axisThreeDriveAlert = axisSubInputGroup->addProperty(tr("Drive Alert"));
	QtProperty *axisThreePosLimit = axisSubInputGroup->addProperty(tr("Positive Limit"));
	QtProperty *axisThreeNegLimit = axisSubInputGroup->addProperty(tr("Negative Limit"));
	QtProperty *axisFourDriveAlert = axisSubInputGroup->addProperty(tr("Drive Alert"));
	QtProperty *axisFourPosLimit = axisSubInputGroup->addProperty(tr("Positive Limit"));
	QtProperty *axisFourNegLimit = axisSubInputGroup->addProperty(tr("Negative Limit"));
	axisOneDigitalInput->addSubProperty(axisOneDriveAlert);
	axisOneDigitalInput->addSubProperty(axisOnePosLimit);
	axisOneDigitalInput->addSubProperty(axisOneNegLimit);
	axisTwoDigitalInput->addSubProperty(axisTwoDriveAlert);
	axisTwoDigitalInput->addSubProperty(axisTwoPosLimit);
	axisTwoDigitalInput->addSubProperty(axisTwoNegLimit);
	axisThreeDigitalInput->addSubProperty(axisThreeDriveAlert);
	axisThreeDigitalInput->addSubProperty(axisThreePosLimit);
	axisThreeDigitalInput->addSubProperty(axisThreeNegLimit);
	axisFourDigitalInput->addSubProperty(axisFourDriveAlert);
	axisFourDigitalInput->addSubProperty(axisFourPosLimit);
	axisFourDigitalInput->addSubProperty(axisFourNegLimit);

	QtGroupPropertyManager
		*axisSubOutputGroup = new QtGroupPropertyManager;
	QtProperty *axisOnePlanEquivalent = axisSubOutputGroup->addProperty(tr("Planner Equivalent"));
	QtProperty *axisOneProfileEquivalent = axisSubOutputGroup->addProperty(tr("Profile Equivalent"));
	QtProperty *axisOneGeneralOutput = axisSubOutputGroup->addProperty(tr("General Output"));
	QtProperty *axisOneServoEnable = axisSubOutputGroup->addProperty(tr("Servo Enable"));
	QtProperty *axisOneStatClear = axisSubOutputGroup->addProperty(tr("Stat Clear"));
	QtProperty *axisTwoPlanEquivalent = axisSubOutputGroup->addProperty(tr("Planner Equivalent"));
	QtProperty *axisTwoProfileEquivalent = axisSubOutputGroup->addProperty(tr("Profile Equivalent"));
	QtProperty *axisTwoGeneralOutput = axisSubOutputGroup->addProperty(tr("General Output"));
	QtProperty *axisTwoServoEnable = axisSubOutputGroup->addProperty(tr("Servo Enable"));
	QtProperty *axisTwoStatClear = axisSubOutputGroup->addProperty(tr("Stat Clear"));
	QtProperty *axisThreePlanEquivalent = axisSubOutputGroup->addProperty(tr("Planner Equivalent"));
	QtProperty *axisThreeProfileEquivalent = axisSubOutputGroup->addProperty(tr("Profile Equivalent"));
	QtProperty *axisThreeGeneralOutput = axisSubOutputGroup->addProperty(tr("General Output"));
	QtProperty *axisThreeServoEnable = axisSubOutputGroup->addProperty(tr("Servo Enable"));
	QtProperty *axisThreeStatClear = axisSubOutputGroup->addProperty(tr("Stat Clear"));
	QtProperty *axisFourPlanEquivalent = axisSubOutputGroup->addProperty(tr("Planner Equivalent"));
	QtProperty *axisFourProfileEquivalent = axisSubOutputGroup->addProperty(tr("Profile Equivalent"));
	QtProperty *axisFourGeneralOutput = axisSubOutputGroup->addProperty(tr("General Output"));
	QtProperty *axisFourServoEnable = axisSubOutputGroup->addProperty(tr("Servo Enable"));
	QtProperty *axisFourStatClear = axisSubOutputGroup->addProperty(tr("Stat Clear"));
	axisOneDigitalOutput->addSubProperty(axisOnePlanEquivalent);
	axisOneDigitalOutput->addSubProperty(axisOneProfileEquivalent);
	axisOneDigitalOutput->addSubProperty(axisOneGeneralOutput);
	axisOneDigitalOutput->addSubProperty(axisOneServoEnable);
	axisOneDigitalOutput->addSubProperty(axisOneStatClear);
	axisTwoDigitalOutput->addSubProperty(axisTwoPlanEquivalent);
	axisTwoDigitalOutput->addSubProperty(axisTwoProfileEquivalent);
	axisTwoDigitalOutput->addSubProperty(axisTwoGeneralOutput);
	axisTwoDigitalOutput->addSubProperty(axisTwoServoEnable);
	axisTwoDigitalOutput->addSubProperty(axisTwoStatClear);
	axisThreeDigitalOutput->addSubProperty(axisThreePlanEquivalent);
	axisThreeDigitalOutput->addSubProperty(axisThreeProfileEquivalent);
	axisThreeDigitalOutput->addSubProperty(axisThreeGeneralOutput);
	axisThreeDigitalOutput->addSubProperty(axisThreeServoEnable);
	axisThreeDigitalOutput->addSubProperty(axisThreeStatClear);
	axisFourDigitalOutput->addSubProperty(axisFourPlanEquivalent);
	axisFourDigitalOutput->addSubProperty(axisFourProfileEquivalent);
	axisFourDigitalOutput->addSubProperty(axisFourGeneralOutput);
	axisFourDigitalOutput->addSubProperty(axisFourServoEnable);
	axisFourDigitalOutput->addSubProperty(axisFourStatClear);

	QtStringPropertyManager *filterTimeManager = new QtStringPropertyManager;
	QtLineEditFactory *filterTimeFactory = new QtLineEditFactory;
	QRegExp exp("[0-9]{0,8}");
	QtProperty *driveAlertFilter_1 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *driveAlertFilter_2 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *driveAlertFilter_3 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *driveAlertFilter_4 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *posLimitFilter_1 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *posLimitFilter_2 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *posLimitFilter_3 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *posLimitFilter_4 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *negLimitFilter_1 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *negLimitFilter_2 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *negLimitFilter_3 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *negLimitFilter_4 = filterTimeManager->addProperty(tr("Filter Time"));
	QtProperty *planAlpha_1 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *planBeta_1 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *profileAlpha_1 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *profileBeta_1 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *planAlpha_2 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *planBeta_2 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *profileAlpha_2 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *profileBeta_2 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *planAlpha_3 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *planBeta_3 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *profileAlpha_3 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *profileBeta_3 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *planAlpha_4 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *planBeta_4 = filterTimeManager->addProperty(tr("Beta"));
	QtProperty *profileAlpha_4 = filterTimeManager->addProperty(tr("Alpha"));
	QtProperty *profileBeta_4 = filterTimeManager->addProperty(tr("Beta"));
	filterTimeManager->setRegExp(driveAlertFilter_1, exp);
	filterTimeManager->setRegExp(driveAlertFilter_2, exp);
	filterTimeManager->setRegExp(driveAlertFilter_3, exp);
	filterTimeManager->setRegExp(driveAlertFilter_4, exp);
	filterTimeManager->setRegExp(posLimitFilter_1, exp);
	filterTimeManager->setRegExp(posLimitFilter_2, exp);
	filterTimeManager->setRegExp(posLimitFilter_3, exp);
	filterTimeManager->setRegExp(posLimitFilter_4, exp);
	filterTimeManager->setRegExp(negLimitFilter_1, exp);
	filterTimeManager->setRegExp(negLimitFilter_2, exp);
	filterTimeManager->setRegExp(negLimitFilter_3, exp);
	filterTimeManager->setRegExp(negLimitFilter_4, exp);
	filterTimeManager->setRegExp(planAlpha_1, exp);
	filterTimeManager->setRegExp(planBeta_1, exp);
	filterTimeManager->setRegExp(profileAlpha_1, exp);
	filterTimeManager->setRegExp(profileBeta_1, exp);
	filterTimeManager->setRegExp(planAlpha_2, exp);
	filterTimeManager->setRegExp(planBeta_2, exp);
	filterTimeManager->setRegExp(profileAlpha_2, exp);
	filterTimeManager->setRegExp(profileBeta_2, exp);
	filterTimeManager->setRegExp(planAlpha_3, exp);
	filterTimeManager->setRegExp(planBeta_3, exp);
	filterTimeManager->setRegExp(profileAlpha_3, exp);
	filterTimeManager->setRegExp(profileBeta_3, exp);
	filterTimeManager->setRegExp(planAlpha_4, exp);
	filterTimeManager->setRegExp(planBeta_4, exp);
	filterTimeManager->setRegExp(profileAlpha_4, exp);
	filterTimeManager->setRegExp(profileBeta_4, exp);
	propertyBrowser->setFactoryForManager(filterTimeManager, filterTimeFactory);

	axisOneDriveAlert->addSubProperty(driveAlertFilter_1);
	axisOneDriveAlert->addSubProperty(driveAlertActivate_1);
	axisOneDriveAlert->addSubProperty(driveAlertInvert_1);
	axisOnePosLimit->addSubProperty(posLimitFilter_1);
	axisOnePosLimit->addSubProperty(posLimitActivate_1);
	axisOnePosLimit->addSubProperty(posLimitInvert_1);
	axisOneNegLimit->addSubProperty(negLimitFilter_1);
	axisOneNegLimit->addSubProperty(negLimitActivate_1);
	axisOneNegLimit->addSubProperty(negLimitInvert_1);
	axisOnePlanEquivalent->addSubProperty(planAlpha_1);
	axisOnePlanEquivalent->addSubProperty(planBeta_1); 
	axisOneProfileEquivalent->addSubProperty(profileAlpha_1);
	axisOneProfileEquivalent->addSubProperty(profileBeta_1);
	axisOneGeneralOutput->addSubProperty(gOActivated_1);
	axisOneGeneralOutput->addSubProperty(gOInverted_1);
	axisOneServoEnable->addSubProperty(servoEnableActivated_1);
	axisOneServoEnable->addSubProperty(servoEnableInverted_1);
	axisOneStatClear->addSubProperty(statClearActivated_1);
	axisOneStatClear->addSubProperty(statClearInverted_1);

	axisTwoDriveAlert->addSubProperty(driveAlertFilter_2);
	axisTwoDriveAlert->addSubProperty(driveAlertActivate_2);
	axisTwoDriveAlert->addSubProperty(driveAlertInvert_2);
	axisTwoPosLimit->addSubProperty(posLimitFilter_2);
	axisTwoPosLimit->addSubProperty(posLimitActivate_2);
	axisTwoPosLimit->addSubProperty(posLimitInvert_2);
	axisTwoNegLimit->addSubProperty(negLimitFilter_2);
	axisTwoNegLimit->addSubProperty(negLimitActivate_2);
	axisTwoNegLimit->addSubProperty(negLimitInvert_2);
	axisTwoPlanEquivalent->addSubProperty(planAlpha_2);
	axisTwoPlanEquivalent->addSubProperty(planBeta_2);
	axisTwoProfileEquivalent->addSubProperty(profileAlpha_2);
	axisTwoProfileEquivalent->addSubProperty(profileBeta_2);
	axisTwoGeneralOutput->addSubProperty(gOActivated_2);
	axisTwoGeneralOutput->addSubProperty(gOInverted_2);
	axisTwoServoEnable->addSubProperty(servoEnableActivated_2);
	axisTwoServoEnable->addSubProperty(servoEnableInverted_2);
	axisTwoStatClear->addSubProperty(statClearActivated_2);
	axisTwoStatClear->addSubProperty(statClearInverted_2);

	axisThreeDriveAlert->addSubProperty(driveAlertFilter_3);
	axisThreeDriveAlert->addSubProperty(driveAlertActivate_3);
	axisThreeDriveAlert->addSubProperty(driveAlertInvert_3);
	axisThreePosLimit->addSubProperty(posLimitFilter_3);
	axisThreePosLimit->addSubProperty(posLimitActivate_3);
	axisThreePosLimit->addSubProperty(posLimitInvert_3);
	axisThreeNegLimit->addSubProperty(negLimitFilter_3);
	axisThreeNegLimit->addSubProperty(negLimitInvert_3);
	axisThreeNegLimit->addSubProperty(negLimitInvert_3);
	axisThreePlanEquivalent->addSubProperty(planAlpha_3);
	axisThreePlanEquivalent->addSubProperty(planBeta_3);
	axisThreeProfileEquivalent->addSubProperty(profileAlpha_3);
	axisThreeProfileEquivalent->addSubProperty(profileBeta_3);
	axisThreeGeneralOutput->addSubProperty(gOActivated_3);
	axisThreeGeneralOutput->addSubProperty(gOInverted_3);
	axisThreeServoEnable->addSubProperty(servoEnableActivated_3);
	axisThreeServoEnable->addSubProperty(servoEnableInverted_3);
	axisThreeStatClear->addSubProperty(statClearActivated_3);
	axisThreeStatClear->addSubProperty(statClearInverted_3);

	axisFourDriveAlert->addSubProperty(driveAlertFilter_4);
	axisFourDriveAlert->addSubProperty(driveAlertActivate_4);
	axisFourDriveAlert->addSubProperty(driveAlertInvert_4);
	axisFourPosLimit->addSubProperty(posLimitFilter_4);
	axisFourPosLimit->addSubProperty(posLimitActivate_4);
	axisFourPosLimit->addSubProperty(posLimitInvert_4);
	axisFourNegLimit->addSubProperty(negLimitFilter_4);
	axisFourNegLimit->addSubProperty(negLimitActivate_4);
	axisFourNegLimit->addSubProperty(negLimitInvert_4);
	axisFourPlanEquivalent->addSubProperty(planAlpha_4);
	axisFourPlanEquivalent->addSubProperty(planBeta_4);
	axisFourProfileEquivalent->addSubProperty(profileAlpha_4);
	axisFourProfileEquivalent->addSubProperty(profileBeta_4);
	axisFourGeneralOutput->addSubProperty(gOActivated_4);
	axisFourGeneralOutput->addSubProperty(gOInverted_4);
	axisFourServoEnable->addSubProperty(servoEnableActivated_4);
	axisFourServoEnable->addSubProperty(servoEnableInverted_4);
	axisFourStatClear->addSubProperty(statClearActivated_4);
	axisFourStatClear->addSubProperty(statClearInverted_4);

	propertyBrowser->addProperty(axisOne);
	propertyBrowser->addProperty(axisTwo);
	propertyBrowser->addProperty(axisThree);
	propertyBrowser->addProperty(axisFour);
}
