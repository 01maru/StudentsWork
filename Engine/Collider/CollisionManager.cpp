#include "CollisionManager.h"
#include "Collision.h"
#include "MeshCollider.h"
#include "RayCast.h"
#include "QueryCallBack.h"
#include <cassert>

#include "Object3D.h"

using namespace MyMath;

MNE::CollisionManager* MNE::CollisionManager::GetInstance()
{
    static CollisionManager instance;
    return &instance;
}

void MNE::CollisionManager::RemoveCollider()
{
    colliders_.remove_if([](std::unique_ptr<BaseCollider>& collider) {
        return collider->GetDeleteFlag();
        });
}

void MNE::CollisionManager::CheckCollision(BaseCollider* collider, uint16_t attribute)
{
    //  削除予定だったら処理しない
    if (collider->GetDeleteFlag() == TRUE) return;

    for (auto& col : colliders_)
    {
        //  削除予定だったら処理しない
        if (col->GetDeleteFlag() == TRUE) continue;

        //  判定予定の属性ではなかったら処理しない
        if (!(col->attribute_ & attribute)) continue;

        //  球同士
        if (collider->GetShapeType() == COLLISIONSHAPE_SPHERE &&
            col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            //  球同士の当たり判定
            Sphere* sphereA = dynamic_cast<Sphere*>(collider);
            Sphere* sphereB = dynamic_cast<Sphere*>(col.get());
            Vector3D inter;
            if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter)) {
                CollisionInfo colAInfo(collider->GetObject3D(), collider, inter);
                CollisionInfo colBInfo(col->GetObject3D(), col.get(), inter);
                collider->OnCollision(colBInfo);
                col->OnCollision(colAInfo);
            }
        }
        else if (collider->GetShapeType() == COLLISIONSHAPE_PLANE &&
            col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Plane* plane = dynamic_cast<Plane*>(collider);
            Sphere* sphere = dynamic_cast<Sphere*>(col.get());
            Vector3D inter;
            if (Collision::CheckSphere2Plane(*sphere, *plane, &inter)) {
                CollisionInfo colBInfo(col->GetObject3D(), col.get(), inter);
                CollisionInfo colAInfo(collider->GetObject3D(), collider, inter);
                collider->OnCollision(colBInfo);
                col->OnCollision(colAInfo);
            }
        }
        else if (collider->GetShapeType() == COLLISIONSHAPE_SPHERE &&
            col->GetShapeType() == COLLISIONSHAPE_PLANE) {
            Sphere* sphere = dynamic_cast<Sphere*>(collider);
            Plane* plane = dynamic_cast<Plane*>(col.get());
            Vector3D inter;
            if (Collision::CheckSphere2Plane(*sphere, *plane, &inter)) {
                CollisionInfo colBInfo(col->GetObject3D(), col.get(), inter);
                CollisionInfo colAInfo(collider->GetObject3D(), collider, inter);
                collider->OnCollision(colBInfo);
                col->OnCollision(colAInfo);
            }
        }
        else if (collider->GetShapeType() == COLLISIONSHAPE_MESH &&
            col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(collider);
            Sphere* sphere = dynamic_cast<Sphere*>(col.get());
            Vector3D inter;
            if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
                CollisionInfo colBInfo(col->GetObject3D(), col.get(), inter);
                CollisionInfo colAInfo(collider->GetObject3D(), collider, inter);
                collider->OnCollision(colBInfo);
                col->OnCollision(colAInfo);
            }
        }
        else if (collider->GetShapeType() == COLLISIONSHAPE_SPHERE &&
            col->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col.get());
            Sphere* sphere = dynamic_cast<Sphere*>(collider);
            Vector3D inter;
            if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
                CollisionInfo colBInfo(col->GetObject3D(), col.get(), inter);
                CollisionInfo colAInfo(collider->GetObject3D(), collider, inter);
                collider->OnCollision(colBInfo);
                col->OnCollision(colAInfo);
            }
        }
    }
}

bool MNE::CollisionManager::Raycast(const Ray& ray, RayCast* hitinfo, float maxDistance)
{
    return Raycast(ray, 0xFFFF, hitinfo, maxDistance);
}

bool MNE::CollisionManager::Raycast(const Ray& ray, uint16_t attribute, RayCast* hitinfo, float maxDistance)
{
    bool ans = false;
    auto itr = colliders_.begin();
    std::forward_list<std::unique_ptr<BaseCollider>>::iterator itr_hit;
    float distance = maxDistance;
    Vector3D inter;

    for (; itr != colliders_.end(); ++itr) {
        BaseCollider* colA = itr->get();

        if (!(colA->attribute_ & attribute)) continue;

        if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphere = dynamic_cast<Sphere*>(colA);
            float tempDis;
            Vector3D tempInter;

            if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDis, &tempInter)) continue;
            if (tempDis >= distance) continue;

            ans = true;
            distance = tempDis;
            inter = tempInter;
            itr_hit = itr;
        }
        else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE) {
            Plane* plane = dynamic_cast<Plane*>(colA);
            float tempDis;
            Vector3D tempInter;

            if (!Collision::CheckRay2Plane(ray, *plane, &tempDis, &tempInter)) continue;
            if (tempDis >= distance) continue;

            ans = true;
            distance = tempDis;
            inter = tempInter;
            itr_hit = itr;
        }
        else if (colA->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

            float tempDistance;
            Vector3D tempInter;
            if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
            if (tempDistance >= distance) continue;

            ans = true;
            distance = tempDistance;
            inter = tempInter;
            itr_hit = itr;
        }
    }

    if (ans && hitinfo) {
        hitinfo->distance = distance;
        hitinfo->inter = inter;
        hitinfo->collider = itr_hit->get();
        hitinfo->object = hitinfo->collider->GetObject3D();
    }

    return ans;
}

Vector3D MNE::CollisionManager::CollisionStage(const Sphere& sphere)
{
    Vector3D ans = sphere.center_;

    if (stageCollider_ == nullptr) return ans;

    Vector3D pos(sphere.center_.x, 0.0f, sphere.center_.z);
    pos -= Vector3D(stageCollider_->center_.x, 0.0f, stageCollider_->center_.y);

    if (pos.GetLength() <= (stageCollider_->radius_ - sphere.radius_)) return ans;

    Vector3D dir = pos.GetNormalize();

    ans = Vector3D(stageCollider_->center_.x, sphere.center_.y, stageCollider_->center_.y) + dir * (stageCollider_->radius_ - sphere.radius_);
    return ans;
}

void MNE::CollisionManager::QuerySphere(const Sphere& sphere, QueryCallBack* callback, uint16_t attribute)
{
    assert(callback);

    for (auto it = colliders_.begin(); it != colliders_.end(); ++it) {
        BaseCollider* col = it->get();

        if (!(col->attribute_ & attribute)) continue;

        // 球
        if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphereB = dynamic_cast<Sphere*>(col);

            Vector3D tempInter;
            Vector3D tempReject;
            if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;
            info.reject = tempReject;

            if (!callback->OnQueryHit(info)) return;
        }
        // メッシュ
        else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

            Vector3D tempInter;
            Vector3D tempReject;
            if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;
            info.reject = tempReject;

            if (!callback->OnQueryHit(info)) return;
        }
    }
}
