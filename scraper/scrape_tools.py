#!/usr/bin/env python
#This is the main tool to query/crawl wikipedia and output triples.

import sys
import json
import urllib
from serialization_helpers import *

def get_result_bunch(cursor):
    """execute a query with the specified cursor [=kindof offset in the database]"""

    service_url = 'https://www.googleapis.com/freebase/v1/mqlread'
    query = [{
        "id": [],
        "mid": [],
       #"name": "Malaria",
       #  "*": [{}],
         #"topic": [{}],
         #"/common/topic/notable_for": [{}],
         "type": {
           "key": { "namespace": "/chemistry", "limit": 1 },
           "*":[],
         "limit": 1
         },
         "limit": 100 
        }]

    params = {
        "query" :     json.dumps(query),
        "key" : "AIzaSyCgJBUnif7NrPqwY_loGQXo9OGaPPHTjA0",
        "cursor" : str(cursor),
        }
    #print "#", params 
    url = service_url + '?' + urllib.urlencode(params)
    #print "#", url
    response = json.loads(urllib.urlopen(url).read())
    #assert ('result' in response)
    if ('result' in response):
        return  response['cursor'],response['result']
    else:
        print response
        quit()
   #elif 'error' in response:
   #    print response['error']['message']
   #    return  cursor,[]



def data_to_files(data, pf, ef, target_node_inclusion=None):
    """take the json result and print it to the two files"""
    key_exclusion_list = [
            "url./common/topic/topic_equivalent_webpage" ,   #just crap translations of the webpage
            "object./type/object/attribution",               #we don't care about the author
                        ]
    key_inclusion_list  = [
        #  "practitioner./education/field_of_study/academics_in_this_field",
           "narrower_than./food/ingredient/more_general_ingredient",
           "broader_than./medicine/drug_brand/canonical_drug",
        #  "object./freebase/valuenotation/is_reviewed",
        #  "origin./people/person/place_of_birth",
        #  "subclass_of./people/cause_of_death/parent_cause_of_death",
           "part_of./biology/gene/genome",
        #  "certification./award/award_winner/awards_won",
        #  "member_of./people/person/religion",
           "category./biology/organism_classification/rank",
           "part_of./medicine/drug/marketed_formulations",
           "narrower_than./base/skosbase/skos_concept/broader",
        #  "symbolizes./location/location_symbol_relationship/administrative_division",
        #  "category./people/person/ethnicity",
        #  "practitioner./business/industry/companies",
        #  "part_of./people/family_member/family",
           "superclass_of./business/product_ingredient/generalization_of",
        #  "location./people/place_lived/location",
        #  "certification./award/ranking/list",
           "narrower_than./chemistry/isotope/isotope_of",
        #  "member_of./royalty/chivalric_order_membership/order",
           "medicine./medicine/disease_cause/diseases",
        #  "peer_of./influence/peer_relationship/peers",
           "narrower_than./base/skosbase/vocabulary_equivalent_topic/broader_concept",
           "status./medicine/drug/legal_status",
           "category./medicine/drug/mechanism_of_action",
        #  "member_of./organization/organization_membership/organization",
           "broader_than./base/skosbase/skos_concept/narrower",
           "category./chemistry/chemical_compound/classifications",
           "broader_than./chemistry/chemical_element/isotopes",
           "medicine./medicine/medical_treatment/side_effects",
        #  "fiction_link./fictional_universe/fictional_substance/fictional_object",
        #  "parent./people/person/parents",
           "narrower_than./medicine/drug_ingredient/more_general_ingredient",
           "discovered_by./chemistry/chemical_element/discoverer",
           "broader_than./food/ingredient/more_specific_ingredient",
           "superclass_of./engineering/material/subclass",
           "superclass_of./chemistry/chemical_classification/lower_classifications",
           "narrower_than./medicine/drug/canonical_brands",
           "certification./medicine/drug/pregnancy_category",
        #  "title./royalty/noble_title_tenure/noble_title",
           "subclass_of./chemistry/chemical_classification/higher_classifications",
           "category./chemistry/chemical_element/chemical_series",
        #  "created./organization/organization_founder/organizations_founded",
           "discovered./chemistry/element_discoverer/discovered",
        #  "created_by./organization/organization/founders",
        #  "property./common/topic/image",
           "medicine./medicine/condition_prevention_factors/conditions_this_may_prevent",
        #  "skos./base/skosbase/skos_concept/in_scheme",
        #  "peer_of./people/marriage/spouse",
        #  "member_of./education/academic/departments_old",
        #  "peer_of./education/academic/advisees",
           "medicine./medicine/risk_factor/diseases",
           "location./biology/gene/chromosome",
        #  "peer_of./education/academic/advisors",
           "category./biology/gene_group_membership/group",
        #  "category./location/location_symbol_relationship/Kind_of_symbol",
        #  "created./book/author/works_written",
        #  "location./people/deceased_person/place_of_burial",
           "subclass_of./engineering/material/parent_material_class",
        #  "title./business/employment_tenure/title",
           "category./chemistry/chemical_element/periodic_table_block",
        #  "certification./education/education/degree",
        #  "object./freebase/review_item/flag",
           "category./biology/gene_ontology_group/group_type",
        #  "object./dataworld/gardening_hint/replaces",
        #  "means_of_demise./people/deceased_person/cause_of_death",
        #  "owner./business/company_brand_relationship/company",
           "created./law/inventor/inventions",
           "use_permitted_by./spaceflight/rocket_engine_fuel/rocket_engines",
           "part_of./medicine/drug_ingredient/active_ingredient_of_formulation",
        #  "leader_of./organization/organization_board_membership/organization",
           "created_by./law/invention/inventor",
        #  "practitioner_of./people/person/profession",
        #  "medicine./medicine/medical_treatment/contraindications",
        #  "member_of./people/person/nationality",
        #  "child./people/person/children",
        #  "member_of./base/skosbase/skos_concept/in_scheme",
        #  "category./people/person/gender",
           "subclass_of./business/product_ingredient/variety_of",
           "subclass_of./biology/organism_classification/higher_classification",
           "part_of./organization/organization_relationship/parent",
        #  "object./type/object/attribution",
           "subclass_of./biology/gene_ontology_group/broader_group",
           "peer_of./base/skosbase/vocabulary_equivalent_topic/equivalent_concept",
           "use_permitted_by./spaceflight/rocket_engine_oxidizer/rocket_engines",
           "peer_of./freebase/equivalent_topic/equivalent_type",
        #  "object./freebase/valuenotation/has_value",
           "broader_than./base/schemastaging/food_extra/narrower",
           "broader_than./medicine/drug_ingredient/more_specific_ingredient",
           "broader_than./base/skosbase/vocabulary_equivalent_topic/narrower_concept",
        #  "member_of./business/employment_tenure/company",
           "category./medicine/drug/drug_class",
           "made_of./food/nutrition_fact/nutrient",
        #  "peer_of./people/sibling_relationship/sibling",
        #  "leader_of./organization/leadership/organization",
           "medicine./medicine/medical_treatment/used_to_treat",
        #  "created./people/person/quotations",
        #  "practitioner_of./education/education/major_field_of_study",
        #  "member_of./education/education/institution",
           "means_of_demise./chemistry/isotope_decay/decay_mode",
        #  "practitioner_of./organization/organization_board_membership/role",
        #  "object./freebase/valuenotation/has_no_value", 
            ]

    for p in data:
        if 'name' in p:
            nodeprops={'name':p['name']}
        else:
            nodeprops={}
        if 'output' in p and 'all' in p['output']:
            for k,v in p['output']['all'].items():
                for vi in v:
                    if type(vi) == dict and 'mid' in vi:
                        if not k in key_inclusion_list: continue  #skip excluded keys
                        if not target_node_inclusion is None:
                            if vi['mid'] in target_node_inclusion:
                                ef.add_entry(p['mid'],k,vi['mid'])
                        else:
                            ef.add_entry(p['mid'],k,vi['mid'])
                    else:
                        #if the value is a number, store it as such.
                        try:
                            if not "timepoint" in k:
                                foo = float(vi)
                            else:
                                foo = vi
                        except ValueError:
                            foo = (vi)

                        nodeprops[k] = foo
        pf.add_entry(p['mid'],nodeprops)

def get_topic_for_id(concept_id):
    """execute a query with the specified cursor [=kindof offset in the database]"""

    service_url = 'https://www.googleapis.com/freebase/v1/search'
    params = {
        #"id": concept_id,
        "filter" : "(any " + " ".join(["id:" + x for x in concept_id]) + ")",
        "output": "(all)",
        "key" : "AIzaSyCgJBUnif7NrPqwY_loGQXo9OGaPPHTjA0",
        "limit" : 150,
     }
    url = service_url + '?' + urllib.urlencode(params)
    response = json.loads(urllib.urlopen(url).read())
    if ('result' in response):
        return  response['result']
    elif 'error' in response:
        print response['error']['message']
        return  None
    else:
        print response
        quit()
