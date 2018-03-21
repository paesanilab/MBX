import mbnrgplugin
from simtk.openmm import app
from simtk import unit

ATOM_TYPES = {
    "MBPol-O" : 0,
    "MBPol-H" : 1,
    "MBPol-M" : 2,
    "MBPol-Cl" : 3,
}

## @private
class MBnrgForceGenerator:

    def __init__(self):

        self.types1 = []
        self.types2 = []
        self.types3 = []

    @staticmethod
    def parseElement(element, forceField):
        generator = MBnrgForceGenerator()
        forceField.registerGenerator(generator)

        # <MBPolOneBodyForce>
        #     <Residue class1="OW" class2="HW" class3="HW" />
        # </MBPolOneBodyForce>

        for MBnrgForce_template in element.findall('MBnrgForce'):
            types = forceField._findAtomTypes(MBnrgForce_template.attrib, 3)
            if None not in types:

                generator.types1.append(types[0])
                generator.types2.append(types[1])
                generator.types3.append(types[2])

            else:
                outputString = self.__class__ + ": error getting types: %s %s %s" % (
                                    MBnrgForce_template.attrib['class1'],
                                    MBnrgForce_template.attrib['class2'],
                                    MBnrgForce_template.attrib['class3'])
                raise ValueError(outputString)

    def createForce(self, sys, data, nonbondedMethod, nonbondedCutoff, args):

#        methodMap = {app.NoCutoff:mbpolplugin.MBPolOneBodyForce.NonPeriodic,
#                     app.PME:mbpolplugin.MBPolOneBodyForce.Periodic,
#                     app.CutoffPeriodic:mbpolplugin.MBPolOneBodyForce.Periodic,
#                     app.CutoffNonPeriodic:mbpolplugin.MBPolOneBodyForce.NonPeriodic}

        existing = [sys.getForce(i) for i in range(sys.getNumForces())]
        existing = [f for f in existing if type(f) == mbnrgplugin.MBnrgForce]

#        if nonbondedMethod not in methodMap:
#            raise ValueError('Illegal nonbonded method for MBPolOneBodyForce')

        if len(existing) == 0:
            force = mbnrgplugin.MBnrgForce()
            sys.addForce(force)
        else:
            force = existing[0]

#        force.setNonbondedMethod(methodMap[nonbondedMethod])

#       Add monomer information to python pair list
        residue_index_pair = []
        for i in data.atoms:
            id_residue = i.residue.id
            name_residue = i.residue.name
            pair = [name_residue, id_residue]
            
            if not pair in residue_index_pair:
                residue_index_pair.append([name_residue, id_residue])

#       Add the monomer order to the force
        v = mbnrgplugin.vectorstring()
        #v = mbnrgplugin.vectori()
        for i in residue_index_pair:
            v.push_back(i[0])
        force.addMonomerList(v)
        
# Here is where I will need to add the information of monomers
# Then add it to the force class, so it knows everything it needs
#        for i in range(len(data.angles)):
#            angle = data.angles[i]
#            atom1 = data.atoms[angle[0]]
#            atom2 = data.atoms[angle[1]]
#            atom3 = data.atoms[angle[2]]
#            if atom1.residue.name == 'HOH' and atom2.residue.name == 'HOH' and atom3.residue.name == 'HOH':
#                # FIXME loop through all residues of MBPolOneBodyForce and match their name
#                v = mbpolplugin.vectori()
#                v.push_back(atom2.index)
#                v.push_back(atom1.index)
#                v.push_back(atom3.index)
#                force.addOneBody(v);

app.forcefield.parsers["MBnrgForce"] = MBnrgForceGenerator.parseElement
