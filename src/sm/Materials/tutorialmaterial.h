/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef tutorialmaterial_h
#define tutorialmaterial_h

#include "Materials/isolinearelasticmaterial.h"
#include "Materials/structuralms.h"

///@name Input fields for TutorialMaterial
//@{
#define _IFT_TutorialMaterial_Name "tutorialmaterial"
#define _IFT_TutorialMaterial_yieldstress "sigy"
#define _IFT_TutorialMaterial_hardeningmoduli "h"
//@}

namespace oofem {
class Domain;

/**
 * This class implements a isotropic plastic linear material (J2 plasticity condition is used).
 * in a finite element problem.
 * Both kinematic and isotropic hardening is supported.
 */
class TutorialMaterial : public IsotropicLinearElasticMaterial
{
protected:

    /// Hardening modulus.
    double H;

    /// Initial (uniaxial) yield stress.
    double sig0;
    
    
public:
    TutorialMaterial(int n, Domain * d);
    virtual ~TutorialMaterial();

    virtual IRResultType initializeFrom(InputRecord *ir);
    virtual const char *giveInputRecordName() const { return _IFT_TutorialMaterial_Name; }
    virtual const char *giveClassName() const { return "TutorialMaterial"; }
    virtual bool isCharacteristicMtrxSymmetric(MatResponseMode rMode) { return true; }

    virtual MaterialStatus *CreateStatus(GaussPoint *gp) const;
    const void giveDeviatoricProjectionMatrix(FloatMatrix &answer);
    // stress computation methods
    virtual void giveRealStressVector_3d(FloatArray &answer, GaussPoint *gp, const FloatArray &reducedE, TimeStep *tStep);
    
    virtual void give3dMaterialStiffnessMatrix(FloatMatrix &answer, MatResponseMode mode, GaussPoint *gp, TimeStep *tStep);
    
protected:
    
    static double computeJ2InvariantOf(const FloatArray &sigV);
    //double evaluateYieldFunction();

    static void computeSphDevPartOf(const FloatArray &sigV, FloatArray &sigSph, FloatArray &sigDev); 
};





class TutorialMaterialStatus : public StructuralMaterialStatus
{
protected:

    /// Temporary plastic strain (the given iteration)
    FloatArray tempPlasticStrain;
    
    ///  Last equilibriated plastic strain (end of last time step)
    FloatArray plasticStrain;
    
    FloatArray tempDevTrialStress;

    double tempKappa;
    double kappa;

public:
    TutorialMaterialStatus(int n, Domain * d, GaussPoint * g);
    virtual ~TutorialMaterialStatus(){};

    const FloatArray &givePlasticStrain() { return plasticStrain; }

    void letTempPlasticStrainBe(FloatArray values) { tempPlasticStrain = std :: move(values); }

    const double &giveKappa() { return this->kappa; }

    void letTempKappaBe(double value) { tempKappa = value; }
    
    void letTempDevTrialStressBe(FloatArray values) { tempDevTrialStress = std :: move(values); }
    const FloatArray &giveTempDevTrialStress() { return tempDevTrialStress; }
    
    virtual const char *giveClassName() const { return "TutorialMaterialStatus"; }
    
    virtual void initTempStatus();

    virtual void updateYourself(TimeStep *tStep);

    
    
    // semi optional methods
    //virtual void printOutputAt(FILE *file, TimeStep *tStep);

    
    //virtual contextIOResultType saveContext(DataStream *stream, ContextMode mode, void *obj = NULL);
    //virtual contextIOResultType restoreContext(DataStream *stream, ContextMode mode, void *obj = NULL);

};





} // end namespace oofem
#endif // j2mat_h