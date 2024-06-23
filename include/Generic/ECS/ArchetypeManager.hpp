template <typename T, typename ...Types>
static int addEntity(const int& entityTypeId, T&& arg, Types &&...args) {
    int entityId = entityIDsAllocator.getName();
    assertNoAbort([&entityTypeId, entityTypeMask]()->bool {return EntityManager::entityTypeExists(entityTypeMask); },
        "ArchetypeManager :: addEntity :: entity type not found");
    int entityLocalId = archetypes[entityTypeId].addEntity(entityTypeId, arg, args...);
    entityLocations[entityId] = { entityTypeId, entityLocalId };
    return entityId;
}