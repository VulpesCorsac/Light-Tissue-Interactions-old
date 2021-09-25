#!/bin/bash
./Tests/AnglesTests
echo ""
./Tests/BugerLambertTests
echo ""
./Tests/ReflectanceTests
echo ""
./Tests/MediumInterfaceTests
echo ""
./Tests/MediumGlassTests
echo ""
./Tests/MediumConstantTests
echo ""
./Tests/MediumLinearTests
echo ""
./Tests/MediumArbitraryTests
echo ""
./Tests/MediumUtilsTests
echo ""
./Tests/MediumFactoryTests
echo ""
./Tests/Mesh3Tests
echo ""
./Tests/SettingsImporterErrorsTests
echo ""
./Tests/SettingsImporterMediumTypeTests
echo ""
./Tests/TestAD
echo ""
./Tests/TestIAD
echo ""
./Tests/TestMC
echo ""

./Tests/unit_tests
